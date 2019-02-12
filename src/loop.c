#include "keyboard.h"
#include "lcd.h"
#include "led.h"
#include "state.h"
#include "system_timer.h"
#include "sound.h"
#include "loop.h"
#include "aduc812.h"
#include "stdio.h"

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
			sound_error_signal();
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

void sound_final_signal() {
		//set_volume(7);
		set_freq(A_NOTE_4);
		delay_ms(500);
		//set_volume(0);
}

void sound_error_signal() {
		//set_volume(7);
		set_freq(C_NOTE_4);
		delay_ms(500);
		//set_volume(0);
}

void run_iteration(iteration_parameters* params) {
	volatile char* strr = (char*)malloc( sizeof(char) * 80 );
	if(params != NULL) {
		lcd_clear();
		lcd_set_string("Waiting for delay");
		delay_ms(params -> delay * 1000);
        lcd_clear();
		lcd_set_string("IN PROGRESS");
		delay_ms_with_leds(params -> processing_time * 1000, 255 >> (8 - params -> intensity));
		leds(0);
		sound_final_signal();
		lcd_clear();
    }
}

int main(void) {
	volatile char* strr = (char*)malloc( sizeof(char) * 80 );
    iteration_parameters* params;
    fsm_init();
	EA = 1;

    while(1) {
        switch(get_current_state()) {
            case INIT_STATE:
                params = allocate_params();
				lcd_clear();
				lcd_set_string(welcome_msg);
				mode_selection_step(bracket_for_user_io());
                break;

            case DEFERRED_MODE_SELECTED_STATE:
				lcd_clear();
                lcd_set_string(enter_delay_msg);
                params -> delay = bracket_for_user_io();
				handle_event(DELAY_SPECIFIED_EVENT);
                break;

            case STRICT_MODE_SELECTED_STATE:
				lcd_clear();
                lcd_set_string(enter_intensity_msg);
                params -> intensity = bracket_for_user_io();
				
				if (params -> intensity > 8) {
					sound_error_signal();
					handle_event(FALLBACK_TO_INIT_EVENT);
				} else {
				    handle_event(INTENSITY_SPECIFIED_EVENT);
				}
				
                break;

            case INTENSITY_SELECTED_STATE:
				lcd_clear();
                lcd_set_string(enter_processing_time_msg);
                params -> processing_time = bracket_for_user_io();			
                handle_event(WORK_TIME_SPECIFIED_EVENT);
                break;

            case DELAY_SELECTED_STATE:
                lcd_clear();
                lcd_set_string(enter_intensity_msg);
                params -> intensity = bracket_for_user_io();
                handle_event(INTENSITY_SPECIFIED_EVENT);
                break;

            case PROGRESS_STATE:
                run_iteration(params);
                handle_event(FALLBACK_TO_INIT_EVENT);
                break;
        }
    }
}