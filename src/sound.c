#include "aduc812.h"
#include "isr.h"
#include "max.h"

#define VOL_MIN 0
#define VOL_MAX 0b00011100

#define TF2_VECTOR 5

static unsigned char freq_h;
static unsigned char freq_l;
static volatile unsigned char volume = 0x0;
static volatile unsigned char sound_on = 0;

// Handler for timer 2 interrupts, generate sound of set frequency
void T2_ISR() __interrupt {
    ET2 = 0;

    if (!sound_on) {
        write_max(ENA, (volume & 7) << 2);
        sound_on = 1;
    } else {
        write_max(ENA, VOL_MIN);
        sound_on = 0;
    }

    TH2 = freq_h;
    TL2 = freq_l;

    TF2 = 0;
    ET2 = 1;
}

static short note(int hz)
{
    return 0xFFFF - MCLKIN / ((hz * 2)) / 12;
}

static char note_h(int hz)
{
    return (note(hz) >> 8) & 0xFF;
}

static char note_l(int hz)
{
    return note(hz) & 0xFF;
}

void set_freq(int hz)
{
    ET2 = 0;
    freq_h = note_h(hz);
    freq_l = note_l(hz);
    ET2 = 1;
}

void set_volume(char value) {
    volume = value > 7 ? 7 : value;
}

void sound_init()
{
    set_vector(0x2000 + TF2_VECTOR * 8 + 3, (void *) T2_ISR);

    // Set T2 to timer mode
    CNT2 = 0;

    // Lowest priority
    PT2 = 0;

    // Start values for T2
    TH2 = 0x00;
    TL2 = 0x00;

    // Start T2
    TR2 = 1;

    // Enable T2
    ET2 = 1;
}