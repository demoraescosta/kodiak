#include <debug/debug.h>
#include <debug/printf.h>
#include <fb/fb.h>

void kputchar(char c)
{
    if (c == '\n')
        kputchar('\r');

    framebuffer_putchar(c);
}

void kprintf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf_(fmt, args);
	va_end(args);
}
