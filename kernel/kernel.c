#include <stddef.h>
#include <stdint.h>
#include <limine.h>

#include <types.h>
#include <klibc/string.h>

#include <fb/fb.h>
#include <sys/gdt.h>
#include <sys/isr.h>
#include <debug/debug.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// halt catch and fire
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

void kmain(void) {
    asm("cli");

    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        hcf();
    }

	struct limine_framebuffer *limine_fb = framebuffer_request.response->framebuffers[0];

	framebuffer_t fb = {0};
	fb.address = (uint32_t *)limine_fb->address;
	fb.pitch = limine_fb->pitch;
	fb.width = limine_fb->width;
	fb.height = limine_fb->height;
	fb.text_color = 0x00eee8d5;
	fb.text_x = 0;
	fb.text_y = 0;
	fb.bg_color = 0x00124560;
	fb.color_masks[0].offset = limine_fb->red_mask_shift;
	fb.color_masks[0].length = limine_fb->red_mask_size;
	fb.color_masks[1].offset = limine_fb->green_mask_shift;
	fb.color_masks[1].length = limine_fb->green_mask_size;
	fb.color_masks[2].offset = limine_fb->blue_mask_shift;
	fb.color_masks[2].length = limine_fb->blue_mask_size;

	framebuffer_init(&fb);

    framebuffer_puts("[INFO]: Initialized framebuffer\n");

    gdt_init();
    framebuffer_puts("[INFO]: GDT Initialized\n");
    isr_init();
    framebuffer_puts("[INFO]: IDT populated\n");

    framebuffer_puts("[INFO]: Kernel setup done\n");

    framebuffer_puts("Hello Kodiak!\n");

    kprintf("printf test %s\n", "hello printf!");

    hcf();
}
