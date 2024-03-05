
#include "stack.h"
#include "state_machine.h"
Stack *create_stack()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->Head = NULL;
    s->AfterOutputBuffer = create_string();
    s->OutputBuffer = create_string();
    s->InputBuffer = create_string();
    s->DefMacros = create_macro_list();
    s->ExpandCalls = 0;
    return s;
}

Node *create_node()
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->Str = create_string();
    n->Next = NULL;
    n->BeforeNode = 0;
    n->AfterNode = 0;
    return n;
}

bool isEmpty(Stack *s)
{
    return s->Head == NULL;
}
void stack_push(Stack *st, Node *n)
{
    if (st->Head != NULL)
    {
        n->Next = st->Head;
    }
    st->Head = n;
    // If Head is Null, make this head and next null
}

// Called if head stack node's string is empty
bool stack_pop(Stack *st)
{
    if (isEmpty(st))
    {
        return false;
    }
    else
    {
        Node *newHead = st->Head->Next;
        free_node(st->Head);
        st->Head = newHead;
    }
    return true;
}

void push_string_to_stack(String *str, Stack *s)
{
    Node *newNode = create_node();
    string_putString(newNode->Str, str->Content, str->Size);
    stack_push(s, newNode);
}

void expand_push_string_to_stack(String *str, Stack *s, int before_node, int after_node)
{
    Node *newNode = create_node();
    string_putString(newNode->Str, str->Content, str->Size);
    
        newNode->BeforeNode = before_node;
        newNode->AfterNode = after_node;
    

    stack_push(s, newNode);
}

void process_stack(Stack *s)
{
    if (s->Head->BeforeNode)
    {
        string_putString(s->Head->Str, s->AfterOutputBuffer->Content, s->AfterOutputBuffer->Size); // Add output from after stack to the end of the before stack
        s->AfterOutputBuffer = clear_string(s->AfterOutputBuffer);
    }

    // Current node has finsihed being processed and gets popped
    if (s->Head->Str->Idx == s->Head->Str->Size)
    {
        if (s->Head->AfterNode)
        {
            s->ExpandCalls -= 1;
        }
        stack_pop(s);
        return;
    }
    // String *inputBuffer = create_string(); // just saves name really
    Macro *readMacro = create_macro();

    while (s->Head->Str->Idx < s->Head->Str->Size)
    {

        if (tick(s->Head->Str->Content[s->Head->Str->Idx], readMacro, s)) // returns true if Macro has been read
        {

            process_macro(readMacro, s);

            if (string_compare(readMacro->Name, "def") || string_compare(readMacro->Name, "include") )
            {
                readMacro = create_macro();
            }
            else if ( string_compare(readMacro->Name, "continue"))
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    
    // tick(EOF, readMacro, s);
}

// returns true if macro adds a node to the stack
void process_macro(Macro *readMacro, Stack *s)
{
    // int i =1;
    // int test = strncmp(readMacro->Name->Content, "def", readMacro->Name->Size);

    // if name=def, add it to list of macros
    if (string_compare(readMacro->Name, "def"))
    {
        if(isDefined(s->DefMacros, readMacro->Arg_1)){
            DIE("%s", "Macro already defined");
        }
        add_macro_list(readMacro, s->DefMacros);

        return; // don't want to free the processed macro
    }
    else if (string_compare(readMacro->Name, "undef"))
    {
        remove_macro_from_list(readMacro->Arg_1, s->DefMacros);
    }
    else if (string_compare(readMacro->Name, "if")) // if name=if, arg1 empty?, if true add arg2 to stack, else add arg3 of m to stack
    {
        if (readMacro->Arg_1->Size == 0)
        {
            push_string_to_stack(readMacro->Arg_3, s);
        }
        else
        {
            push_string_to_stack(readMacro->Arg_2, s);
        }
    }
    else if (string_compare(readMacro->Name, "ifdef"))
    {
        if (isDefined(s->DefMacros, readMacro->Arg_1))
        {
            push_string_to_stack(readMacro->Arg_2, s);
        }
        else
        {
            push_string_to_stack(readMacro->Arg_3, s);
        }
    }
    else if (string_compare(readMacro->Name, "include")) // if name=include, read in file name in arg1 and add to list
    {
        string_putchar(readMacro->Arg_1, '\0'); // add null terminator to file name

        FILE *f = fopen(readMacro->Arg_1->Content, "r");

        String *fileOutput = create_string();
        include_file(f, fileOutput);
        // print_string(fileOutput);

        // Copy the rest of include line
        String *headString = s->Head->Str; // String of node at head of stack
        for(int i = headString->Idx; i < headString->Size; i++){
            // if(headString->Content[i] == '\n'){
            //     break;
            // }
            string_putchar(fileOutput, headString->Content[i]); // add end of node string to included file content
        }
        // print_string(fileOutput);
        s->Head->Str = fileOutput;
        free_string(headString);
        // return;
    }
    else if (string_compare(readMacro->Name, "expandafter")) // create node with  arg2 for str and then another node with arg1 as str
    {

        expand_push_string_to_stack(readMacro->Arg_1, s, 1, 0);
        expand_push_string_to_stack(readMacro->Arg_2, s, 0, 1);
        s->ExpandCalls += 1;
    }
    else
    { //Process defined macros

        if (isDefined(s->DefMacros, readMacro->Name))
        {

            Macro *defMacro = find_macro(s->DefMacros, readMacro->Name);
            process_defined_macro(defMacro, readMacro->Arg_1, s);
        }
        else
        {
            DIE("%s", "Macro is not defined");
        }
    }

    free_macro(readMacro); // as long as its not def
}

void process_defined_macro(Macro *defined_macro, String *replacement, Stack *s)
{
    String *newString = create_string();
    String *macroDefintion = defined_macro->Arg_2;
    char c;

    for (int i = 0; i < macroDefintion->Size; i++)
    {
        c = *((macroDefintion->Content) + i);
        if (c == '#')
        {
            string_putString(newString, replacement->Content, replacement->Size);
        }
        else
        {
            string_putchar(newString, c);
        }
    }

    push_string_to_stack(newString, s);
    free_string(newString);
}

void free_node(Node *n)
{

    free_string(n->Str);
    free(n);
}

void free_stack(Stack *s)
{
    // free(s->Head); I think this already gets freed when the node it points to gets freed using freeNode
    free_string(s->OutputBuffer);
    free_string(s->InputBuffer);
    free(s);
}