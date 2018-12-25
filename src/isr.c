#include "aduc812.h"
#include "max.h"

void set_vector(unsigned char __xdata * address, void * vector)
{
    unsigned char __xdata * tmp_vector;

    *address = 0x02;

    tmp_vector = (unsigned char __xdata *) (address + 1);
    *tmp_vector = (unsigned char) ((unsigned short) vector >> 8);
    ++tmp_vector;
    *tmp_vector = (unsigned char) vector;
}
