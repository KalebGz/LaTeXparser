#include "macro.h"
#include "string.h"

Macro *create_macro()
{
    Macro *m = (Macro *)malloc(sizeof(Macro));
    m->Name = create_string();
    m->Arg_1 = create_string();
    m->Arg_2 = create_string();
    m->Arg_3 = create_string();
    return m;
}

MacroList *create_macro_list()
{
    MacroList *ml = (MacroList *)malloc(sizeof(MacroList));
    ml->size = 0;
    ml->capacity = 10;
    ml->list = (Macro **)malloc(sizeof(Macro *) * ml->capacity);
    return ml;
}

void add_macro_list(Macro *dm, MacroList *ml)
{
    if (ml->size == ml->capacity)
    {
        double_macro_list(ml);
    }
    // (*(ml->list) + ml->size) = dm;
    // *(ml->list + ml->size) = dm;
    ml->list[ml->size] = dm;
    ml->size += 1;
}

void remove_macro_from_list(String *macro_name, MacroList *ml)
{
    Macro *m = find_macro(ml, macro_name);
    free_string(m->Name);
    String *newName = create_string();
    string_putcharArr(newName, "\\Undefed Macro");
    m->Arg_1 = newName;
}

void double_macro_list(MacroList *ml)
{

    ml->list = DOUBLE(ml->list, ml->capacity); //TEST: capacity is already doubled
    // ml->capacity *= 2;
}
Macro *find_macro(MacroList *ml, String *name)
{
    for (int i = 0; i < ml->size; i++)
    {
        Macro *currMacro = *((ml->list) + i);

        if (string_compare(currMacro->Arg_1, "\\Undefed Macro"))
        { 
            continue;
        }

        if (string_compare_size(currMacro->Arg_1, name->Content, name->Size))
        {
            return currMacro;
        }
    }
    return NULL;
}

bool isDefined(MacroList *ml, String *name)
{
    return (find_macro(ml, name) != NULL);
}

int returnNumArgs(String *name)
{
    if (string_compare(name, "def") || (string_compare(name, "expandafter")))
    {
        return 2;
    }
    else if (string_compare(name, "if") || string_compare(name, "ifdef"))
    {
        return 3;
    }
    else if (string_compare(name, "if"))
    {
        return 3;
    }
    else
    { // include
        return 1;
    }
}

void free_macro(Macro *m)
{
    // if (m->Arg_1 != NULL)
    // {
    //     free_string(m->Arg_1);
    // }
    // if (m->Arg_2 != NULL)
    // {
    //     free_string(m->Arg_2);
    // }
    // if (m->Arg_2 != NULL)
    // {
    //     free_string(m->Arg_2);
    // }

    // String are all created so they shouldn't be NULL
    free_string(m->Arg_1);
    free_string(m->Arg_2);
    free_string(m->Arg_3);
}