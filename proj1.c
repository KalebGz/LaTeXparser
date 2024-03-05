#include "proj1.h"
#include "string.h"
#include "stack.h"
#include "macro.h"
#include "state_machine.h"

int main(int argc, char **argv)
{

    // printf("argc: %d\n", argc);
    // printf("argv: %s\n", argv[1]);

    // test string
    String *inputString = create_string();
    char c;

    // Process inputString from stdin or from files
    if (argc < 2)
    {
        while ((c = getchar()) != EOF)
        {
            remove_comments(inputString, c);
        }
        remove_comments(inputString, EOF); // notifies state machine that can print \ if in escaped state
    }
    else // Process Files
    {
        for (int i = 1; i < argc; i++)
        {
            FILE *f;

            f = fopen(argv[i], "r");
            include_file(f, inputString);
        }
    }

    // print_string(inputString);
    // return -1;

    Stack *expansionStack = create_stack();
    Node *inputNode = create_node();
    string_putString(inputNode->Str, inputString->Content, inputString->Size);
    stack_push(expansionStack, inputNode);

    // Expand it here
    while (!isEmpty(expansionStack))
    {
        process_stack(expansionStack);
    }

    string_putchar(expansionStack->OutputBuffer, '\0');
    printf("%s", expansionStack->OutputBuffer->Content);

    // print_string(expansionStack->OutputBuffer);

    /*
     Stack TESTS

        Node *node2 = create_node();
        string_putstring(node2->Str, "Blah blah sdhfakjsdhf");
        stack_push(expansionStack, node2);

        print_string(expansionStack->Head->Str);

        stack_pop(expansionStack);
        print_string(expansionStack->Head->Str);
        stack_pop(expansionStack);
        free_stack(expansionStack);

        free_string(inputString);
    
    */
}
