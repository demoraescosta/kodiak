#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limine.h>

#include "types.h"

#include "terminal/flanterm.h"
#include "terminal/backends/fb.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

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

// TODO: this sucks!!
size_t strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
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

    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    /* for (size_t i = 0; i < 255; i++) { */
    /*     volatile uint32_t *fb_ptr = framebuffer->address; */
    /*     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff; */
    /* } */

    struct flanterm_context* ctx = flanterm_fb_init(
            NULL, NULL, // TODO: implement malloc and free
            framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch,
            framebuffer->red_mask_size, framebuffer->red_mask_shift,
            framebuffer->green_mask_size, framebuffer->green_mask_shift,
            framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
            NULL, // no canvas
            NULL, NULL, // default ansi colors
            NULL, NULL, // "
            NULL, NULL, // "
            NULL, 0, 0, 0, // deafult font, font widht, height and spacing
            0, 0, // scale_x, scale_y
            8 // margin
            );
    
    const char* msg = " \n    ###   s###s  ### \n  ##   ##s     s#   # \n  #   #          # ## \n   +#s   .  _   * # \n    #      s =s    # \n    #     #  * #   # \n     #b    #___# _s# \n    #  #s_s#####s  # \n   # -#_            # \n  # -#_             #s \n #          #        # \ns#     #     # s      # \n#       #     #       # \n#    #####o    ##      ###s \n s  #     #      s     #   # \n  ##s    #s_      #     #  # \n     ####   #####s s####s## ";
    flanterm_write(ctx, msg, strlen(msg));

    const char* msg2 = "\n\nHello Kodiak!\n";
    flanterm_write(ctx, msg2, strlen(msg2));
    hcf();
}
