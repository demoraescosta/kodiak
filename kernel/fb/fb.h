#ifndef FB_H
#define FB_H

#include <fb/flanterm/flanterm.h>
#include "types.h"

// straight up copied from linux code
struct fb_bitfield {
	uint32_t offset;	/* beginning of bitfield	*/
	uint32_t length;	/* length of bitfield		*/
	uint32_t msb_right; /* != 0 : Most significant bit is */
						/* right */
};

typedef struct framebuffer {
	u16 width, height;
	u16 text_width, text_height;
	u32 text_color;
    u32* address; // TODO: add front and back buffer
	u32 bg_color;
	size_t pitch, bpp;
	size_t text_x, text_y;
	struct fb_bitfield color_masks[3];
	struct flanterm_context *ctx;
} framebuffer;

void framebuffer_init(framebuffer* fb);
void framebuffer_clear(u32 fg, u32 bg);
void framebuffer_puts(char* string);
void framebuffer_putchar(char c);

#endif
