NAME=kodiak

PREFIX=i686-elf

ASM=$(PREFIX)-as

CC=$(PREFIX)-gcc
CFLAGS=-std=gnu99 -Wall -Wextra

boot.o:
	$(ASM) boot.s -o $@

kernel.o:
	$(CC) -c kernel.c -o $@	 $(CFLAGS) -ffreestanding -O2

$(NAME).bin: boot.o kernel.o
	$(CC) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

$(NAME).iso: $(NAME).bin 
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$(NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso isodir

kodiak-iso: $(NAME).iso

clean:
	rm *.o
	rm *.iso
	rm $(NAME).bin
	rm -r isodir
