#pragma once

// disable interrupts and save previous value of EA
#define __DISABLE_INTERRUPTS__ unsigned char __TEMP_EA = EA; EA = 0; 

// set previous value of EA
#define __ENABLE_INTERRUPTS__ EA = __TEMP_EA;
