#include "keyboard.h"

void disable_user_keyboard_io() { return; }

void enable_user_keyboard_io() { return; }

/*
    In dependence of current state, user can enter number with different semantic values:
    1) intensity of work ( 0 - 7 )
    2) working time ( 1 - 1800 )
    3) mode (1 - strict mode, 2 - deferred mode)
    4) delay time (1 - 1800 )
*/

unsigned int receive_user_number() { return 0; }