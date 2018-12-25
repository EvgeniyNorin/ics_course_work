#include "aduc812.h"
#include "drivers_utils.h"
#include "isr.h"
#include "timer_driver.h"

typedef struct _timer {
    unsigned char is_configured:4;
    unsigned char is_handler_used:4;
} timer_t;

static volatile timer_t timers[3];

// used for idle interrupt handling
static void STUB_ISR() __interrupt {}

int timer_init(
    const tnum_t timer_number,
    const gate_control_t gate,
    const timer_type_t type,
    const timer_mode_t mode
) {
    int status = 0;
    __DISABLE_INTERRUPTS__

    if (timer_number == TIMER_0 || timer_number == TIMER_1) {
        unsigned char cfg = (gate << 3) | (type << 2) | mode;
        unsigned char mask = 0;

        if (timer_number == TIMER_0) {
            mask = 0xF0;
        } else {
            mask = 0x0F;
            cfg <<= 4;
        }

        // TMOD structure:
        // [GATE_T1:1, C/T_T1:1, MOD_T1:2, GATE_T0:1, C/T_T0:1, MOD_T0:2]
        TMOD = (TMOD & mask) | cfg;
        timers[timer_number].is_configured = 1;

        if (timer_number == TIMER_0) {
            ET0 = 1;
            TR0 = 1;
        } else {
            ET1 = 1;
            TR1 = 1;
        }
    } else {
        status = 3; // not implemented
    }

    __ENABLE_INTERRUPTS__
    return status;
}

int timer_teardown(const tnum_t timer_number) {
    int status = 0;
    __DISABLE_INTERRUPTS__
    timers[timer_number].is_configured = 0;

    if (timer_number == TIMER_0 || timer_number == TIMER_1) {
        unsigned cfg = 0x8; // 1000b
        unsigned char mask = 0;

        if (timer_number == TIMER_0) {
            TR0 = 0;
            mask = 0xF0;
        } else {
            TR1 = 0;
            mask = 0x0F;
            cfg <<= 4;
        }

        TMOD = (TMOD & mask) | cfg;
    } else {
        status = 3; // not implemented
    }

    __ENABLE_INTERRUPTS__
    return status;
}

int set_timer_handler(const tnum_t timer_number, void* handler) {
    int status = 0;

    unsigned char __xdata* address = 0;

    __DISABLE_INTERRUPTS__


        if (timer_number == TIMER_0) {
            address = 0x2000 + TF0_VECTOR * 8 + 3;
        } else if (timer_number == TIMER_1) {
            address = 0x2000 + TF1_VECTOR * 8 + 3;
        } else {
            status = 3;
        }

        if (address) {
            set_vector(address, handler);
            timers[timer_number].is_handler_used = 1;
        }

    __ENABLE_INTERRUPTS__
    return status;
}

int clear_timer_handler(const tnum_t timer_number) {
    int status = 0;
    unsigned char __xdata* address = 0;

    __DISABLE_INTERRUPTS__


        if (timer_number == TIMER_0) {
            address = 0x2000 + TF0_VECTOR * 8 + 3;
        } else if (timer_number == TIMER_1) {
            address = 0x2000 + TF1_VECTOR * 8 + 3;
        } else {
            status = 3;
        }

        if (address) {
            set_vector(address, (void *) STUB_ISR);
            timers[timer_number].is_handler_used = 0;
        }

    __ENABLE_INTERRUPTS__
    return status;
}

int set_timer_values(const tnum_t timer_number, const unsigned short value) {
    int status = 0;
    unsigned char h, l;
    __DISABLE_INTERRUPTS__

    h = (value >> 8) & 0xFF;
    l = value & 0xFF;

    switch (timer_number) {
        case TIMER_0:
            TH0 = h;
            TL0 = l;
            break;
        case TIMER_1:
            TH1 = h;
            TL1 = l;
            break;
        default:
            status = 3;
    }

    __ENABLE_INTERRUPTS__
    return status;
}