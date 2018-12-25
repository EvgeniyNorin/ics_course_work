#include "keyboard.h"
#include "lcd.h"
#include "led.h"
#include "state.h"
#include "time.h"
#include "sound.h"
#include "loop.h"

void fsm_init() {
    lcd_init();
    sound_init();
}

void mode_selection_step(uint8_t mode) {
    switch(mode) {
        case 1: 
            handle_event(STRICT_MODE_SELECTED_EVENT);
            break;
        case 2: 
            handle_event(DEFERRED_MODE_SELECTED_EVENT);
            break;
        default:
            force_return_to_init();
            break;
    }
}

uint16_t bracket_for_int(void (*acquire)(), uint16_t (*use)(), void (*release)()) {
    uint16_t result;
    acquire();
    result = use();
    release();
    return result;
}

uint16_t bracket_for_user_io() {
    return bracket_for_int(
        &enable_user_keyboard_io, 
        &receive_user_number, 
        &disable_user_keyboard_io
    );
}

iteration_parameters* allocate_params() {
    iteration_parameters* params = (iteration_parameters*)malloc(sizeof(iteration_parameters));
    params -> delay = 0;
    return params;
}

void loop() {
    iteration_parameters* params;
    while(1) {
        params = allocate_params();
        switch(get_current_state()) {
            case INIT_STATE:
                mode_selection_step(bracket_for_user_io());
                break;

            case DEFERRED_MODE_SELECTED_STATE:
                params -> delay = bracket_for_user_io();
                handle_event(DELAY_SPECIFIED_EVENT);
                break;

            case STRICT_MODE_SELECTED_STATE:
                params -> intensity = bracket_for_user_io();
                handle_event(INTENSITY_SPECIFIED_EVENT);
                break;

            case INTENSITY_SELECTED_STATE:
                params -> processing_time = bracket_for_user_io();    
                handle_event(WORK_TIME_SPECIFIED_EVENT);
                break;

            case PROGRESS_STATE:
                handle_event(FALLBACK_TO_INIT_EVENT);
                break;
        }
    }
}