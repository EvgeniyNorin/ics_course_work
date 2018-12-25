#include "kb_controller.h"

#include "aduc812.h"

#define MAX_LISTENERS_TO_KEY 16

#include "led.h"

static __xdata kbctrl_listener_t subscriptions[KEYS_COUNT];
static kbctrl_listener_t any_key_subscription;
kbctrl_listener_t key_a;
kbctrl_listener_t key_c;

static int kb_listener(keyboard_event_t events[], unsigned int size) {
    int i;

    for (i = 0; i < size; i++) {
        kbctrl_listener_t subscription = 0;
        keyboard_event_t* event = (events + i);
        if (event->state == KEY_STATE_NONE) {
            continue;
        }

        if (key_a && event->code == KEY_A) {
            key_a(event);
        }

        if (key_c && event->code == KEY_C) {
            key_c(event);
        }

        // subscription = subscriptions[event->code];
        // /// debug_show((unsigned char) event->code);
        // //debug_show((unsigned char) event->code);
        // // debug_slowdown();

        // leds((unsigned char) subscription);
        // if (subscription != 0) {
            // subscription(event);
        // }

        if (any_key_subscription != 0) {
            any_key_subscription(event);
        }
    }

    return 0;
}

int kbctrl_init() {
    int i;
    for (i = 0; i < KEYS_COUNT; i++) {
        subscriptions[i] = 0;
    }
    return keyboard_add_event_listener(kb_listener);
}

int kbctrl_teardown() {
    return keyboard_remove_event_listener(kb_listener);
}

int kbctrl_subscribe_one(kbctrl_listener_t listener, key_code_t key) {
    if (subscriptions[key] != 0) {
        return 2;
    }

    subscriptions[key] = listener;

    return 0;
}

int kbctrl_subscribe_any(kbctrl_listener_t listener) {
    if (any_key_subscription != 0) {
        return 2;
    }

    any_key_subscription = listener;

    return 0;
}

int kbctrl_unsubscribe_one(kbctrl_listener_t listener, key_code_t key) {
    if (subscriptions[key] == 0) {
        return 2;
    }

    if (subscriptions[key] != listener) {
        return 3;
    }

    subscriptions[key] = 0;
    return 0;
}

int kbctrl_unsubscribe_any(kbctrl_listener_t listener) {
    if (any_key_subscription == 0) {
        return 2;
    }

    if (any_key_subscription != listener) {
        return 3;
    }

    any_key_subscription = 0;
    return 0;
}
