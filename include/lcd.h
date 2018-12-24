#pragma once

//#include "util.h"
#include "max.h"

void lcd_init();

void lcd_clear();

/*
    Examples of string:
    "Mode 1: Strict, 2: Deferred"
    "Please, enter delay value"
    "Please, enter working time value"
*/
void lcd_set_string(char* str);