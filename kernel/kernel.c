#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limine.h>

#include "types.h"

/* #include "fb/flanterm/flanterm.h" */
/* #include "fb/flanterm/backends/fb.h" */

#include "fb/fb.h"

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

void *memcpy(void *dest, const void *src, size_t n) {
    u8 *pdest = (u8 *)dest;
    const u8 *psrc = (const u8 *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    u8 *p = (u8 *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (u8)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    u8 *pdest = (u8 *)dest;
    const u8 *psrc = (const u8 *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const u8 *p1 = (const u8 *)s1;
    const u8 *p2 = (const u8 *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// halt catch and fire
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

	struct limine_framebuffer *limine_fb = framebuffer_request.response->framebuffers[0];

	framebuffer fb = {0};

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

    framebuffer_puts(" \n    ###   s###s  ### \n  ##   ##s     s#   # \n  #   #          # ## \n   +#s   .  _   ' # \n    #      s =s    # \n    #     #  * #   # \n     #b    #___# _s# \n    #  #s_s#####s  # \n   # -#_            # \n  # -#_             #s \n #          #        # \ns#     #     # s      # \n#       #     #       # \n#    #####o    ##      ###s \n s  #     #      s     #   # \n  ##s    #s_      #     #  # \n     ####   #####s s####s## ");
    framebuffer_puts("\n\nHello Kodiak!\n");

    hcf();
}
