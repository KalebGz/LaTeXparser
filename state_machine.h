#ifndef STATE_MACHINE_INCLUDED // Include guards
#define STATE_MACHINE_INCLUDED

#include "string.h"
#include "stack.h"


// Struct for managing states
typedef enum
{
    state_plaintext,
    state_escape,
    state_macro_name,
    state_macro_arg,
    state_escaped_in_arg,
} state_t;

// Used to switch states
bool tick(char c, Macro *readMacro, Stack *s);

void clear_buffer(Stack *s);
#endif
