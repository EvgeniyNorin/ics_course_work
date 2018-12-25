#include "kb_driver.h"

#include "aduc812.h"
#include "drivers_utils.h"
#include "max.h"
#include "timer_driver.h"

// additionaly used hardware
#define USED_TIMER TIMER_0

// keyboard geometry
#define MATRIX_KEYBOARD_COLUMNS_COUT 4
#define MATRIX_KEYBOARD_ROWS_COUT 4

// driver scheduling
#define SCAN_FREQUENCY 100
#define SCAN_TIMEOUT (0xFFFF - MCLKIN / ( SCAN_FREQUENCY * 12 ))

volatile kb_listener_t on_change;
volatile unsigned int previous_keys[KEYS_COUNT];

static key_state_t determine_key_state(const int prev, const int curr) {
    return prev
        ? (curr ? KEY_STATE_PRESSED : KEY_STATE_UP)
        : (curr ? KEY_STATE_DOWN    : KEY_STATE_NONE);
}

static unsigned char addr_to_code(const int col, const int row) {
    return (row << 2) | col;
}

static unsigned char scan_column(const unsigned char column) {
    write_max(KB, ~(0x1 << column));
    return ~(read_max(KB) >> 4);
}

static unsigned char scan_row(const unsigned char row_value,
        const unsigned int row_num) {
    return (row_value >> row_num) & 0x01;
}

static int T0_ISR() __interrupt {
    unsigned int i, j, any_key = 0;
    static keyboard_event_t events[KEYS_COUNT];
    EA = 0;

    // please refer to SDK 1.1 User Manual, page 15: Keyboard registers
    for (i = 0; i < MATRIX_KEYBOARD_COLUMNS_COUT; ++i) {
        const unsigned char row = scan_column(i);

        for (j = 0; j < MATRIX_KEYBOARD_ROWS_COUT; ++j) {
            const unsigned char code = addr_to_code(i, j);
            const unsigned char curr = scan_row(row, j);
            const unsigned char prev = previous_keys[code];
            const unsigned char state = determine_key_state(prev, curr);

            events[code].code = code;
            events[code].state = state;

            previous_keys[code] = curr;

            any_key |= state;
        }
    }

    // if state is not changed, then we don't need to inform listener about it
    if (any_key != KEY_STATE_NONE) {
        on_change(events, KEYS_COUNT);
    }

    set_timer_values(USED_TIMER, SCAN_TIMEOUT);

    EA = 1;
    return 0;
}

int keyboard_init() {
    int status = 0;
    __DISABLE_INTERRUPTS__

    status = set_timer_handler(USED_TIMER, (void*) T0_ISR);
    if (status) goto keyboard_init_end;

    status = timer_init(
        USED_TIMER,
        GATE_CONTROL__TIMER_ALWAYS, 
        TIMER,
        MODE_16_BIT
    );

keyboard_init_end:
    __ENABLE_INTERRUPTS__
    return status;
}

int keyboard_add_event_listener(kb_listener_t listener) {
    if (on_change != 0) {
        return 2;
    }

    on_change = listener;
    return 0;
}

int keyboard_remove_event_listener(kb_listener_t listener) {
    if (on_change == listener) {
        on_change = 0;
        return 0;
    }

    return 2;
}

int keyboard_teardown() {
    int status = 0;
    __DISABLE_INTERRUPTS__

    status = clear_timer_handler(USED_TIMER);
    if (status) goto keyboard_teardown_end;

    status = timer_teardown(USED_TIMER);
    if (status) goto keyboard_teardown_end;
    on_change = 0;

keyboard_teardown_end:
    __ENABLE_INTERRUPTS__
    return status;
}
