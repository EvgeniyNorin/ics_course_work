#pragma once

#include <stddef.h>

typedef enum {
    INIT_STATE,
    DEFERRED_MODE_SELECTED_STATE,
    STRICT_MODE_SELECTED_STATE,
    INTENSITY_SELECTED_STATE,
    DELAY_SELECTED_STATE,
    PROGRESS_STATE
} state_e;

typedef enum {
    STRICT_MODE_SELECTED_EVENT,
    DEFERRED_MODE_SELECTED_EVENT,
    INTENSITY_SPECIFIED_EVENT,
    DELAY_SPECIFIED_EVENT,
    WORK_TIME_SPECIFIED_EVENT,
    FALLBACK_TO_INIT_EVENT
} event_e;

state_e get_current_state();

state_e handle_event(event_e event);

void force_return_to_init();