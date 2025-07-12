#ifndef GDT_H
#define GDT_H

#include <types.h>

void gdt_init(void);
void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran);

#endif
