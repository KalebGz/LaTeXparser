#include "state_machine.h"
#include "stack.h"
bool tick(char c, Macro *read_macro, Stack *s)
{
    static state_t state = state_plaintext; // static allows us to preserve state even after we leave function

    static int currArgumentNum = 0;
    static int numArguments = 0; // how many arguments lefft to process
    static int numBraces = 0;
    s->Head->Str->Idx += 1; // incremement Idx of read characters
    String *buffer = (s->ExpandCalls == 0)? s->OutputBuffer: s->AfterOutputBuffer;

    switch (state)
    {
    case state_plaintext:
        // Process escapes here
        if (c == '\\')
        {
            state = state_escape;
        }
        else
        {
            string_putchar(buffer, c);
        }
        return false;
    case state_escape:
        if (c == '%' || (c == '\\') || (c == '{') || (c == '}') || (c == '#'))
        {
            printf("%c", c);
        }
        else if (isalnum(c))
        {
            state = state_macro_name;
            string_putchar(s->InputBuffer, c);
        }
        else
        { // no special meaning if not alphanumeric so just print \ + this character
            string_putchar(s->InputBuffer, '\\');
            string_putchar(s->InputBuffer, c);
            state = state_plaintext;
        }
        return false;
    case state_macro_name:
        // Must be alphanumeric
        if (!isalnum(c))
        {
            if (c == '{')
            {
                numArguments = returnNumArgs(s->InputBuffer);
                numBraces += 1;
                currArgumentNum += 1;
                string_putString(read_macro->Name, s->InputBuffer->Content, s->InputBuffer->Size);
                clear_buffer(s);
                state = state_macro_arg;
                return false;
            }
            else
            {
                DIE("%s", "Non-alphanumeric name or space");

                // string_putchar(s->InputBuffer, c); don't want to do this ebcause
            }
        }
        string_putchar(s->InputBuffer, c);
        return false;
    case state_macro_arg:

        if (c == '{')
        {
            numBraces += 1;

            if (numBraces != 1)
            {
                string_putchar(s->InputBuffer, c); // adds a {}
            }
            return false;
        }

        if (c == '}')
        {
            numBraces -= 1;
            if (numBraces != 0)
            {
                string_putchar(s->InputBuffer, c); // adds a {}
            }

            // numArguments -= 1;
            if (numBraces == 0)
            {

                if (currArgumentNum == 1)
                {
                    string_putString(read_macro->Arg_1, s->InputBuffer->Content, s->InputBuffer->Size);
                }
                else if (currArgumentNum == 2)
                {
                    string_putString(read_macro->Arg_2, s->InputBuffer->Content, s->InputBuffer->Size);
                }

                else if (currArgumentNum == 3)
                {
                    string_putString(read_macro->Arg_3, s->InputBuffer->Content, s->InputBuffer->Size);
                }
                clear_buffer(s);
                currArgumentNum += 1;
            }

            // End of macro interpretation
            if (currArgumentNum > numArguments) // macro has been fully read
            {
                state = state_plaintext;
                // string_putchar(s->InputBuffer, c);
                currArgumentNum = 0;
                return true;
            }

            return false;
        }
        // Error Handling
           if (s->Head->Str->Idx == s->Head->Str->Size && numBraces != 0)
        {
            DIE("%s", "Braces not Balanced");
        }

        if (numBraces == 0 && isspace(c)){
            DIE("%s", "Space Detected");
        }
        

        string_putchar(s->InputBuffer, c);
        return false;

        // clear_buffer(s);
    default:
        return false;
    }
}

void clear_buffer(Stack *s)
{
    free_string(s->InputBuffer);
    s->InputBuffer = create_string();
}
