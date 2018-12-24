#include "keyboard.h"
#include "lcd.h"
#include "led.h"
#include "state.h"
#include "time.h"
#include "timer.h"


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

void loop(uint16_t io_timeout) {
    event_e event;
        switch(get_current_state()) {
            case INIT_STATE:
                enable_user_keyboard_io();
                set_timeout_fallback_handler(io_timeout, )
                mode_selection_step(receive_user_number());
                reset_timeout_fallback();
                disable_user_keyboard_io();
                
            
                
        }

}