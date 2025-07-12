#include "idt.h"
#include "gdt.h"

// https://wiki.osdev.org/Interrupt_Descriptor_Table#Example_Code_2
typedef struct idt_entry_t {
	uint16_t offset_lo;
	uint16_t selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t offset_mid;
	uint32_t offset_hi;
	uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	u16	limit;
	u64	base;
} __attribute__((packed)) idt_ptr_t;

#define IDT_MAX_DESCRIPTORS 256
static idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // entries for each int vector

void idt_set_desc(size_t vec, void *handler, uint8_t ist)
{
	uint64_t p = (uint64_t)handler;

	idt[vec].offset_lo = (u16)p;
	idt[vec].selector = 8;
	idt[vec].ist = ist;
	idt[vec].flags = 0x8E;
	idt[vec].offset_mid = (u16)(p >> 16);
	idt[vec].offset_hi = (u32)(p >> 32);
	idt[vec].zero = 0;
}

void idt_reload(void)
{
    idt_ptr_t ptr = {sizeof(idt) - 1, (uint64_t)idt};

	asm volatile("lidtq %0" : : "m"(ptr));
}
