#pragma once

#define C_NOTE_0        16
#define C_SHARP_NOTE_0  17
#define D_NOTE_0        18
#define D_SHARP_NOTE_0  19
#define E_NOTE_0        20
#define F_NOTE_0        21
#define F_SHARP_NOTE_0  23
#define G_NOTE_0        24
#define G_SHARP_NOTE_0  26
#define A_NOTE_0        27
#define A_SHARP_NOTE_0  29
#define B_NOTE_0        30

#define C_NOTE_4        261
#define C_SHARP_NOTE_4  277
#define D_NOTE_4        293
#define D_SHARP_NOTE_4  311
#define E_NOTE_4        329
#define F_NOTE_4        349
#define F_SHARP_NOTE_4  369
#define G_NOTE_4        392
#define G_SHARP_NOTE_4  415
#define A_NOTE_4        440
#define A_SHARP_NOTE_4  466
#define B_NOTE_4        493

// Set sound frequency
void set_freq(int hz);
void set_volume(char x);
void sound_note();
void sound_init();