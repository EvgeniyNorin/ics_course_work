#pragma once

#include <stdint.h>

typedef void (*timeout_handlr)();


/*
    For every user io there is custom timeout, after which we reset FSM to init state.
    This function should set hanlder for case of timeout.     
*/

void set_timeout_fallback_handler(uint16_t seconds, timeout_handlr handler);

/*
    For the case of successful user io this function should reset previously placed timeout handlers.
*/

void reset_timeout_fallback();

