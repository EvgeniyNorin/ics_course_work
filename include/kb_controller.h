#pragma once

#include "kb_driver.h"

typedef int (*kbctrl_listener_t)(keyboard_event_t*);

extern kbctrl_listener_t key_a;
extern kbctrl_listener_t key_c;

int kbctrl_init();
int kbctrl_teardown();

// listener called when specified key is pressed
int kbctrl_subscribe_one(kbctrl_listener_t listener, key_code_t key);

// listener called when any single event is occurred
int kbctrl_subscribe_any(kbctrl_listener_t listener);

int kbctrl_unsubscribe_one(kbctrl_listener_t listener, key_code_t key);

int kbctrl_unsubscribe_any(kbctrl_listener_t listener);
