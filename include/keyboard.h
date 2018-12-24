#pragma once

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
unsigned int receive_user_number();