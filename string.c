#include "string.h"

String *create_string()
{
    String *s = (String *)malloc(sizeof(String));
    // if (s = malloc(sizeof(*s)) )
    if (s)
    {
        s->Idx = 0;
        s->Size = 0;
        s->Capacity = 10;
        s->Content = (char *)malloc(sizeof(char) * 10);
    }
    return s;
}

String* clear_string(String* s){
    free_string(s);
    return create_string();
}
// Add character c to string content
void string_putchar(String *s, char c)
{
    // Doube capactiy of content if too small
    if (s->Size == s->Capacity)
    {
        s->Content = DOUBLE(s->Content, s->Capacity);
    }
    // #define DOUBLE(ptr,nmemb) realloc (ptr, (nmemb *= 2) * sizeof(*ptr))
    *((s->Content) + s->Size) = c;
    s->Size += 1;
}

void string_putcharArr(String *s, char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {
        string_putchar(s, c[i]);
    }
}

void string_putString(String *s, char *c, int size)
{
    for (int i = 0; i < size; i++)
    {
        string_putchar(s, c[i]);
    }
}

void print_string(String *s)
{
    static int escaped = 0;
    for (int i = 0; i < s->Size; i++)
    {
        char c = *((s->Content) + i);

        if(escaped){
            if (c == '%' || (c == '\\') || (c == '{') || (c == '}') || (c == '#')){
                printf("%c", c);
            }else{
                printf("%c", '\\');
                printf("%c", c);
            }
            escaped = 0;
        }

        if(c == '\\'){
            escaped = 1;
        }
        printf("%c", c);
    }
}

// Removes comments from inputted string
void remove_comments(String *input, char c)
{
    static bool inComment = false;
    static bool newLine = false; // checks if on a new line after a comment(ignore spaces)
    static bool escaped = false;

    if (c == EOF)
    {
        if (escaped)
        {
            string_putchar(input, '\\');
            escaped = false;
        }
        inComment = false;
        newLine = false;
        return;
    }
    
    // TODO Edge: Check if there can be a new line character in the middle of a comment
    if (inComment)
    {
        if (c == '\n')
        {
            inComment = false;
            newLine = true;
        }
    }
    else if (newLine)
    {
        if (c == '%') // Deal with multi-line comments
        {
            inComment = true;
            newLine = false;
        }
        else if (!isblank(c)) //isSpace doesn't capture all cases
        {
            newLine = false;
            string_putchar(input, c);
        }
    }
    else if (c == '\\') // Deal with escaped comments
    {
        escaped = true;
    }
    else if (c == '%' && !escaped)
    { // TODO: What does the staff solution do if there is an escape char in the middle fo a comment?
        inComment = true;
    }
    else if (escaped)
    {
        string_putchar(input, '\\');
        string_putchar(input, c);
        escaped = false;
    }
    else
    {
        string_putchar(input, c); //If none of the conditions before are true, add old character to new one
    }


}

bool string_compare(String *s, char *str)
{
    // str could be longer "aaa" and s="aa" ands since we're only check size of s, this would say they were the same
    if (s->Size != strlen(str))
    {
        return false;
    }

    // Make sure their length is equal first
    int res = strncmp(s->Content, str, s->Size);
    if (res == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool string_compare_size(String *s, char *str, int char_size)
{
    // str could be longer "aaa" and s="aa" ands since we're only check size of s, this would say they were the same
    if (s->Size != char_size)
    {
        return false;
    }

    // Make sure their length is equal first
    int res = strncmp(s->Content, str, s->Size);
    if (res == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void include_file(FILE *f, String *s)
{

    char c;
    if (f)
    {
        while (fscanf(f, "%c", &c) != EOF)
        {
            remove_comments(s, c);
        }
        remove_comments(s, EOF); // notifies state machine that can print \ if in escaped state

        fclose(f);
    }
    else
    {
        DIE("%s", "Invalid File");
    }
}

void free_string(String *s)
{
    free(s->Content);
    free(s);
}
