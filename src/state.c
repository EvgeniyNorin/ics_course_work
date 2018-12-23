#include "state.h"

state_e current_state_e = INIT_STATE;

void default_fallback_transition_handler(event_e event) {
    switch(event) {
        case FALLBACK_TO_INIT_EVENT:
            current_state_e = INIT_STATE;
        default: 
            current_state_e = INIT_STATE;
    }
}

void init_state_transitions_handler(event_e event) {
    switch(event) {
        case KEY_ONE_PRESSED_EVENT:     
            current_state_e = STRICT_MODE_SELECTED_STATE;
            break;
        
        case KEY_TWO_PRESSED_EVENT:
            current_state_e = DEFERRED_MODE_SELECTED_STATE;
            break;

        default:
            default_fallback_transition_handler(event);
            break;
    }
}

void strict_mode_state_transition_handler(event_e event) {
    switch(event) {
        case INTENSITY_SPECIFIED_EVENT:
            current_state_e = INTENSITY_SELECTED_STATE;
            break;
        default:
            default_fallback_transition_handler(event);
            break;    
    }
}

void deferred_mode_state_transition_handler(event_e event) {
    switch(event) {
        case DELAY_SPECIFIED_EVENT:
            current_state_e = DELAY_SELECTED_STATE;
            break;
        default:
            default_fallback_transition_handler(event);
            break;    
    }
}

void delay_selected_state_transition_handler(event_e event) {
    switch(event) {
        case INTENSITY_SPECIFIED_EVENT:
            current_state_e = INTENSITY_SELECTED_STATE;
            break;
        default:
            default_fallback_transition_handler(event);
            break;
    }
}

void intensity_selected_state_transition_handler(event_e event) {
    switch(event) {
        case WORK_TIME_SPECIFIED_EVENT:
            current_state_e = PROGRESS_STATE;
            break;
        default:
            default_fallback_transition_handler(event);
            break;
    }
}

state_e handle_event(event_e event) {
    switch (current_state_e) {
        case INIT_STATE:
            init_state_transitions_handler(event);
            break;
    
        case STRICT_MODE_SELECTED_STATE:
            strict_mode_state_transition_handler(event);
            break;

        case DEFERRED_MODE_SELECTED_STATE:
            deferred_mode_state_transition_handler(event);
            break;

        case DELAY_SELECTED_STATE:
            delay_selected_state_transition_handler(event);
            break;

        case INTENSITY_SELECTED_STATE:
            intensity_selected_state_transition_handler(event);
            break;

        case PROGRESS_STATE:
            default_fallback_transition_handler(event);
            break;

    }

    return current_state_e;
}

state_e get_current_state() {
    return current_state_e;
}