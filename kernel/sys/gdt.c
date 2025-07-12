#include <sys/gdt.h>
#include <types.h>

typedef struct gdt_entry_t {
    u16 limit;
    u16 base_low;
    u8  base_middle;
    u8  access;
    u8  flags;
    u8  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_t
{
	u16 limit;
	u64 base;
} __attribute__((packed)) gdt_ptr_t;

// defined in gdt.asm
extern void gdt_load(void*);

gdt_entry_t g_gdt_entries[5] = { 0 };
gdt_ptr_t   g_gdt_ptr;

void gdt_init(void)
{
    // kernel code
	g_gdt_entries[1].access = 0b10011010;
	g_gdt_entries[1].flags  = 0b00100000;

    // kernel data
	g_gdt_entries[2].access = 0b10010010;

    // user data
	g_gdt_entries[3].access = 0b11110010;

    // user code	
    g_gdt_entries[4].access = 0b11111010;
	g_gdt_entries[4].flags  = 0b00100000;

    gdt_ptr_t p = {
        .limit = sizeof(g_gdt_entries) - 1,
        .base  = (u64)&g_gdt_entries[0]
    };

    gdt_load(&p);
}

void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    g_gdt_entries[num].limit = (limit & 0xFFFF);
    g_gdt_entries[num].base_low = (base & 0xFFFF);
    g_gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    g_gdt_entries[num].access = access;
    g_gdt_entries[num].flags = (limit >> 16) & 0x0F;
    g_gdt_entries[num].flags |= gran & 0xF0;
    g_gdt_entries[num].base_high = (base >> 24) & 0xFF;
}
