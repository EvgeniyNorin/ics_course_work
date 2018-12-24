#pragma once

#include <stdint.h>

/*
    input values: integer number from 0 to 7, which corresponds to the number of flashing led lights 
*/

void display_intensity(uint8_t number_of_lights);

/*
    reset any lightning
*/

void reset_lightning();