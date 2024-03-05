#ifndef MACRO_INCLUDED // Include guards
#define MACRO_INCLUDED

#include "string.h"
#include "proj1.h"
// #include "stack.h"


typedef struct macro
{
    String *Name;
    String *Arg_1;
    String *Arg_2;
    String *Arg_3;

} Macro;

// List of defined macros
typedef struct macroList
{
    int size, capacity;
    Macro **list; 
} MacroList;

Macro* create_macro();

Macro* create_def_macro();

MacroList* create_macro_list(); 

void add_macro_list(Macro *dm, MacroList *ml);

void remove_macro_from_list(String *macro_name, MacroList *ml);

void double_macro_list(MacroList *ml);

Macro* find_macro(MacroList *ml, String *name);

bool isDefined(MacroList *ml, String *name);

int returnNumArgs(String *name);

void free_macro(Macro *m);


// 

// typedef enum
// {
//     type_def,
//     type_if,
//     type_ifdef,
//     type_include,
//     type_expandafter,
//     type_definedmac
// } macro_type;


#endif
