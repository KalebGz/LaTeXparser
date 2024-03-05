#ifndef STACK_INCLUDED // Include guards
#define STACK_INCLUDED

#include "proj1.h"
#include "string.h"
#include "macro.h"
// #include "state_machine.h"

typedef struct node
{
    struct node *Next;
    String *Str;
    int BeforeNode;
    int AfterNode;
}Node;

typedef struct stack
{   
    Node *Head;
    String *OutputBuffer;
    String *AfterOutputBuffer;
    String *InputBuffer;
    MacroList *DefMacros;
    int ExpandCalls; 
}Stack;

Stack* create_stack();

Node* create_node();

bool isEmpty(Stack *s);

void stack_push(Stack *st, Node *n);

bool stack_pop(Stack *st);

void push_string_to_stack(String *str, Stack *s);

void expand_push_string_to_stack(String *str, Stack *s, int before_node, int after_node);

void process_stack(Stack *s);

void process_macro(Macro *m, Stack *s);

void process_defined_macro(Macro *defined_macro, String *replacement, Stack *s);

void free_node(Node *n);

void free_stack(Stack *s);

#endif
