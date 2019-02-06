#pragma once

#include <stdint.h>
#include <stdlib.h>

static const char* welcome_msg = "1: Strict, 2: Deferred";

static const char* enter_delay_msg = "Enter delay";

static const char* enter_processing_time_msg = "Enter processing time";

static const char* enter_intensity_msg = "Enter intensity";

typedef struct _iteration_parameters {
    uint16_t delay;
    uint16_t processing_time;
    uint8_t intensity; 
} iteration_parameters;