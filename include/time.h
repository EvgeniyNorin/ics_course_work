#pragma once

#include <stdint.h>

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day; 
} date_time;

date_time* get_real_time();