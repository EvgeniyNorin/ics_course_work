#pragma once

#include "aduc812.h"

#ifndef MOCK_C51

#define KB          0x0
#define DATA_IND    0x1
#define EXT_LO      0x2
#define EXT_HI      0x3
#define ENA         0x4
#define C_IND       0x6
#define SV          0x7

#else

#define KB          (unsigned char *) 0x0
#define DATA_IND    (unsigned char *) 0x1
#define EXT_LO      (unsigned char *) 0x2
#define EXT_HI      (unsigned char *) 0x3
#define ENA         (unsigned char *) 0x4
#define C_IND       (unsigned char *) 0x6
#define SV          (unsigned char *) 0x7

#endif

void write_max(unsigned char __xdata * regnum, unsigned char val);
unsigned char read_max(unsigned char __xdata * regnum);