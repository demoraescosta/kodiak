#ifndef IDT_H
#define IDT_H

#include <types.h>

void idt_set_desc(size_t vec, void *handler, uint8_t ist);
void idt_reload(void);

#endif
