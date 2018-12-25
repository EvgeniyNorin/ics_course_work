#include "lcd.h"



// define cmds
#define CLEAR           0x01
#define HOME            0x02
#define ENTRY_MODE      0x04
#define DISPLAY_CTRL    0x08
#define SHIFT           0x10
#define FUNCTION_SET    0x20
#define RAM_CG          0x40
#define RAM_DD          0x80
#define BF_AC           0x80

// define command options
#define INCR            0x02
#define DISPLAY_SHIFT   0x01
#define DISPLAY_ON      0x04
#define CURSOR_ON       0x02
#define BLINK           0x01
#define DISPLAY         0x08
#define RIGHT           0x04
#define EIGHT_BITS      0x10
#define TWO_LINE        0x08

typedef enum {
    WRITE_MODE = 0x00,
    READ_MODE  = 0x02
} rw_mode_t;

typedef enum {
    CMD_MODE  = 0x00,
    DATA_MODE = 0x04
} cd_mode_t;

void delay(unsigned long ms)
{
    volatile unsigned long i, j;
    for (j = 0; j < ms; j++) {
        for (i = 0; i < 50; i++);
    }
}

unsigned int x;
unsigned int y;

static void write_ctrl(unsigned char any_data) {
    write_max(C_IND, any_data);
}
static void write_data(unsigned char any_data) {
    write_max(DATA_IND, any_data);
}
static unsigned char read_ctrl() {return read_max(C_IND);}
static unsigned char read_data() {return read_max(DATA_IND);}
static void set_ctrl(rw_mode_t rw_mode, cd_mode_t cd_mode, int enable) {
    // I don't know why initial 1000b is required
    // need to check on real hardware
    write_ctrl(0x8 | cd_mode | rw_mode | !!enable);
}

static void send(unsigned char any_data, cd_mode_t cd_mode) {
    write_data(any_data);
    set_ctrl(WRITE_MODE, cd_mode, 1);
    set_ctrl(WRITE_MODE, cd_mode, 0);
}

static unsigned char receive(cd_mode_t cd_mode) {
    // TODO CHECK ENABLE flag here
    // I'm not sure about how it should work for this guy here
    // as far I have understood from 'Read time diagram'
    // (page 43, SDK 1.1. User Manual)
    // E flag should be set by LCD controller itself
    set_ctrl(WRITE_MODE, cd_mode, 0);

    // May be we need to read E from ctrl,
    // but according to the manual (page 17)
    // those three flags are readonly
    return read_data();
}

static int lcd_read_busy_flag() {
    // [BF,AC6,AC5,AC4,AC3,AC2,AC1,AC0]
    int any_data = receive(CMD_MODE);

    return (any_data >> 7) & 0x01;
}

static void wait_busy() {
    // TODO check it, for-cycle based wait might work here better
    // while (lcd_read_busy_flag());
    delay(1);
}

static void shift_pos() {
    if (++x > 15) {
        x = 0;
        y = !y;
    }
}

void lcd_clear() {
    send(CLEAR, CMD_MODE);
    wait_busy();
}

void lcd_set_pos(unsigned char nx, unsigned char ny) {
    const unsigned char display_address = nx | (ny ? 0x40 : 0);

    send(RAM_DD | display_address, CMD_MODE);
    wait_busy();
    x = nx;
    y = ny;
}

void lcd_put_char(unsigned char symbol) {
    send(symbol, DATA_MODE);
    wait_busy();
}

void lcd_print_char(char symbol) {
    lcd_set_pos(x, y);
    lcd_put_char(symbol);
    shift_pos();
}

void lcd_set_string(char* str) { 
	while (*str) {
        lcd_print_char(*str++);
    }
} 

// TODO check waits
// I'm not sure about those waits
void lcd_init() {
    int i;

    // wait after LCD power on (> 15ms)
    for (i = 0; i < 4000; i++)continue;

    send(FUNCTION_SET | EIGHT_BITS, CMD_MODE);

    // wait according to init prootocol (> 4.1ms)
    for (i = 0; i < 1500; i++) continue; 

    send(FUNCTION_SET | EIGHT_BITS, CMD_MODE);
    // wait according to init prootocol (>100ms)
    for (i = 0; i < 16380; i++) continue;

    send(FUNCTION_SET | EIGHT_BITS, CMD_MODE);
    wait_busy();

    send(FUNCTION_SET | EIGHT_BITS, CMD_MODE);
    send(FUNCTION_SET | EIGHT_BITS | TWO_LINE, CMD_MODE);
    send(DISPLAY_CTRL, DATA_MODE); //Display off
    wait_busy();

    lcd_clear();

    send(ENTRY_MODE | INCR, CMD_MODE);
    wait_busy();

    send(DISPLAY_CTRL | DISPLAY_ON, CMD_MODE);
    wait_busy();
}