NAME := kodiak

QEMUFLAGS := -m 2G \
			 -no-reboot \
			 -debugcon stdio \
			 -d int \
			 # -s -S

CC := cc
LD := ld
ASM := nasm

CFLAGS := -g -O2 -pipe 
ASMFLAGS := -g
LDFLAGS :=
LIBS :=

# Toolchain for building limine
HOST_CC := cc
HOST_CFLAGS := -g -O2 -pipe
HOST_CPPFLAGS :=
HOST_LDFLAGS :=
HOST_LIBS :=

CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-red-zone \
    -mcmodel=kernel
    # -mno-sse \
    # -mno-sse2 \

# preprocessor flags
CPPFLAGS := \
    -I kernel \
    -isystem freestnd-c-hdrs \
    $(CPPFLAGS) \
    -DLIMINE_API_REVISION=3 \
    -MMD \
    -MP

ASMFLAGS += \
			 -felf64 \
			 -g \
			 -Wall 

LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T linker.ld

SRCFILES := $(shell cd kernel && find -L * -type f | LC_ALL=C sort)
CFILES := $(filter %.c,$(SRCFILES))
ASFILES := $(filter %.S,$(SRCFILES))
ASMFILES := $(filter %.asm,$(SRCFILES))
OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(ASMFILES:.asm=.asm.o))

.PHONY: all
all: bin/$(NAME)

bin/$(NAME): linker.ld $(OBJ)
	mkdir -p bin/
	$(LD) $(OBJ) $(LDFLAGS) -o $@

obj/%.c.o: kernel/%.c 
	mkdir -p "$(basename $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.S.o: kernel/%.S
	mkdir -p "$(basename $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.asm.o: kernel/%.asm 
	mkdir -p "$$(dirname $@)"
	$(ASM) $(ASMFLAGS) $< -o $@

limine/limine:
	rm -rf limine
	git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1
	$(MAKE) -C limine \
		CC="$(HOST_CC)" \
		CFLAGS="$(HOST_CFLAGS)" \
		CPPFLAGS="$(HOST_CPPFLAGS)" \
		LDFLAGS="$(HOST_LDFLAGS)" \
		LIBS="$(HOST_LIBS)"

$(NAME)-iso: $(NAME).iso
$(NAME).iso: limine/limine bin/$(NAME)
	rm -rf iso_root
	mkdir -p iso_root/boot
	cp -v bin/kodiak iso_root/boot/
	mkdir -p iso_root/boot/limine
	cp -v limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/boot/limine/
	mkdir -p iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(NAME).iso
	./limine/limine bios-install $(NAME).iso
	rm -rf iso_root

run: $(NAME).iso
		qemu-system-x86_64 \
		-M q35 \
		-cdrom $(NAME).iso \
		-boot d \
		$(QEMUFLAGS)

.PHONY: clean
clean:
	rm -rf bin
	rm -rf obj
	rm -rf iso_root
	rm -rf $(NAME).iso
