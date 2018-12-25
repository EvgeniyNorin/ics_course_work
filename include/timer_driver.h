#pragma once

typedef enum _gate_control {
    GATE_CONTROL__TIMER_ALWAYS = 0,
    GATE_CONTROL__TIMER_CONTROLLABLE = 1
} gate_control_t;

typedef enum _timer_type {     
    /* increments on system clock */
    TIMER = 0x0,
    /* counter operation:
     * increment only when corresponding pin is changed
     * e.g. T0 pin for for timer 0. */
    COUNTER = 0x1
} timer_type_t;

typedef enum _timer_mode {
    MODE_13_BIT = 0x00,
    MODE_16_BIT = 0x01,
    AUTORELOAD  = 0x10,
    // MOD 0x11 is not supported by this driver
} timer_mode_t;

typedef enum _tnum {
    TIMER_0 = 0,
    TIMER_1 = 1,
    TIMER_2 = 2
} tnum_t;


int timer_init(
    const tnum_t timer_number,
    const gate_control_t gate,
    const timer_type_t type,
    const timer_mode_t mode
);

int timer_teardown(const tnum_t timer_number);

int set_timer_handler(const tnum_t timer_number, void* handler);
int clear_timer_handler(const tnum_t timer_number);

int set_timer_values(const tnum_t timer_number, const unsigned short value); 
