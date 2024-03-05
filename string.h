#ifndef STRING_INCLUDED // Include guards to stop from  including > 1
#define STRING_INCLUDED
#include "proj1.h"

// How we are dealing with strings
typedef struct String
{
    int Size,  Capacity;
    int Idx; // keeps track of how much of the string has been processed in the stack. probably better if palced in node but oh well
    char *Content;
} String;

String* create_string();

String* clear_string(String* s);

void string_putchar(String *s, char c);

void string_putcharArr(String *s, char *c);

void string_putString(String *s, char *c, int size); //TODO: change *c to another String and remove size param?

void print_string(String *s);

void remove_comments(String *input, char c);

bool string_compare(String *s, char* str);

bool string_compare_size(String *s, char* str, int size);

void include_file(FILE *f, String *s);

void free_string(String *s);
#endif
