#include "io.h"

// read byte (8 bit) from port
u8 inb(u16 port) {
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// output byte (8 bit) to port
void outb(u16 port, u8 val) {
    asm volatile("outb %1, %0" :: "dN"(port), "a"(val));
}

// read short (16 bit) from port
u16 ins(u16 port) {
    u16 rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

// output short (16 bit) to port
void outs(u16 port, u16 value) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

// read long (32 bit) from port
u32 inl(u16 port) {
    u32 rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (port));
    return rv;
}

// output long (32 bit) to port
void outl(u16 port, u32 value) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (port), "a" (value));
}
