#pragma once

#include <stdint.h>

typedef enum {
    WAIT,
    IN_PROGESS,
    FINISHED
} user_kb_io_state;

/*
    Forbid user to enter anything from keyboard (disbale interruptions handling from kb)
*/

void disable_user_keyboard_io();

/*
    Allow user to enter anything from keyboard
*/

void enable_user_keyboard_io();

/*
    In dependence of current state, user can enter numbers with different semantic values.
    The only responsibility of this function is to receive integer from user. 
*/
uint16_t receive_user_number();