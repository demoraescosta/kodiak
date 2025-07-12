#ifndef IO_H
#define IO_H

#include "types.h"

// read byte (8 bit) from port
u8 inb(u16 port);

// output byte (8 bit) to port
void outb(u16 port, u8 value);

// read short (16 bit) from port
u16 ins(u16 port);

// output short (16 bit) to port
void outs(u16 port, u16 value);

// read long (32 bit) from port
u32 inl(u16 port);

// output long (32 bit) to port
void outl(u16 port, u32 value);

#endif
