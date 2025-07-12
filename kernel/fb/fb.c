#include <fb/fb.h>

#include <fb/flanterm/backends/fb.h>
#include "limine.h"

struct framebuffer_t g_fb;
static struct flanterm_context* g_ctx;

void framebuffer_init(framebuffer_t* fb)
{
    g_fb.address = fb->address;
    g_fb.pitch = fb->pitch;
    g_fb.bpp = fb->bpp;
    g_fb.width = fb->width;
    g_fb.height = fb->height;
    g_fb.text_x = fb->text_x;
    g_fb.text_y = fb->text_y;
    g_fb.text_color = fb->text_color;
    g_fb.text_height = fb->height / 16;
    g_fb.text_width = fb->width / 8;
    g_fb.bg_color = fb->bg_color;

    g_fb.color_masks[0] = fb->color_masks[0];
    g_fb.color_masks[1] = fb->color_masks[1];
    g_fb.color_masks[2] = fb->color_masks[2];

    // TODO: add kmalloc and kfree to this
    g_ctx = flanterm_fb_init(NULL, NULL, (void *)fb->address, fb->width,
            fb->height, fb->pitch, fb->color_masks[0].length,
            fb->color_masks[0].offset, fb->color_masks[1].length,
            fb->color_masks[1].offset, fb->color_masks[2].length,
            fb->color_masks[2].offset, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, 0, 0, 1, 1, 1, 0);

    g_fb.ctx = g_ctx;

    framebuffer_clear(fb->text_color, fb->bg_color);
}

// TODO: implement clear function
void framebuffer_clear(u32 fg, u32 bg)
{
    (void)fg;
    (void)bg;
}

// TODO: this sucks!!
size_t strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}

void framebuffer_puts(char* string)
{
    flanterm_write(g_ctx, string, strlen(string));
}

void framebuffer_putchar(char c)
{
    flanterm_write(g_ctx, &c, 1);
}

