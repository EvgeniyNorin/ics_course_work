#pragma once

/*
just to keep in mind how SDK's keyboard look like:

1 2 3 A
4 5 6 B
7 8 9 C
* 0 # D

*/

#define KEYS_COUNT 16

typedef enum _key_code {
    KEY_0        = 0xD, // 1101b row 3 col 1
    KEY_1        = 0x0, // 0000b row 0 col 0
    KEY_2        = 0x1, // 0001b row 0 col 1
    KEY_3        = 0x2, // 0010b row 0 col 2
    KEY_4        = 0x4, // 0100b row 1 col 0
    KEY_5        = 0x5, // 0101b row 1 col 1
    KEY_6        = 0x6, // 0110b row 1 col 2
    KEY_7        = 0x8, // 1000b row 2 col 0
    KEY_8        = 0x9, // 1001b row 2 col 1
    KEY_9        = 0xA, // 1010b row 2 col 2

    KEY_A        = 0x3, // 0011b row 0 col 3
    KEY_B        = 0x7, // 0111b row 1 col 3
    KEY_C        = 0xB, // 1011b row 2 col 3
    KEY_D        = 0xF, // 1111b row 3 col 3

    KEY_ASTERISK = 0xC, // 1100b row 3 col 0
    KEY_SHARP    = 0xE // 1110b row 3 col 2
} key_code_t;

typedef enum _key_state {
    KEY_STATE_NONE    = 0x01,
    KEY_STATE_UP      = 0x02,
    KEY_STATE_DOWN    = 0x03,
    KEY_STATE_PRESSED = 0x04
} key_state_t;

typedef unsigned int keys_pressed_it;

typedef struct _keyboard_event {
    key_code_t code;
    key_state_t state;
} keyboard_event_t;

typedef int (*kb_listener_t)(keyboard_event_t[], unsigned int size);

int keyboard_init();
int keyboard_add_event_listener(kb_listener_t listener);
int keyboard_remove_event_listener(kb_listener_t listener);
int keyboard_teardown();
