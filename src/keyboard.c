#include "keyboard.h"
#include "kb_driver.h"
#include "kb_controller.h"
#include <stdint.h>
#include <stdlib.h>    
#include "lcd.h"
#include "system_timer.h"

#define MAX_BUFFER_LEN 15

volatile user_kb_io_state io_state = WAIT;
volatile char* buffer_for_numbers;
volatile uint8_t buffer_index = 0;

void disable_user_keyboard_io() {
    kbctrl_teardown();
    keyboard_teardown();
 }

void enable_user_keyboard_io() { 
    keyboard_init();
    kbctrl_init();
}


static int handle_keyboard_input(keyboard_event_t* event) {
    io_state = IN_PROGESS;
	if (event->state == KEY_STATE_UP) {
		switch(event -> code) {
			case KEY_0: 
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '0';
				}
				break;

			case KEY_1: 
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '1';
				}
				break;

			case KEY_2: 
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '2';
				}
				break;
			
			case KEY_3:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '3';
				}
				break;

			case KEY_4:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '4';
				}
				break;    

			case KEY_5:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '5';
				}
				break;

			case KEY_6:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '6';
				}
				break;
			
			case KEY_7:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '7';
				}
				break;

			case KEY_8:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '8';
				}
				break;
			
			case KEY_9:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '9';
				}
				break;
			
			case KEY_D:
				if(buffer_index < MAX_BUFFER_LEN) {
					buffer_for_numbers[buffer_index++] = '\0';
				}
				io_state = FINISHED;
				break;
			
			

		}
	}	
}

uint16_t receive_user_number() { 
    buffer_for_numbers = (char*)malloc( sizeof(char) * MAX_BUFFER_LEN );
    kbctrl_subscribe_any(handle_keyboard_input);
    while(io_state != FINISHED) { continue; }
    free(buffer_for_numbers);
    buffer_index = 0;
    io_state = WAIT;  
	
    return (uint16_t)atoi(buffer_for_numbers); 
}