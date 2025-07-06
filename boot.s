.set ALIGN,    1<<0             
.set MEMINFO,  1<<1            
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

// this marks the program as a kernel. these are numbers that the bootloader
// (in this case grub) will read to find the kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// funnily enough the multiboot standard does not define a value for the stack
// pointer, you need to pass it yourself.
.section bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function // i don't like gas syntax
_start:
    mov $stack_top, %esp
    // todo: initialize floating point instructions and extensions here
    // todo: setup the gdt here
    // todo: enable paging 
    call kernel_main
    cli 
1:  hlt
    jmp 1b

.size _start, . - _start // gas syntax again

