; bits 64

global gdt_load

gdt_load:
    lgdt [rdi]
    push 0x8
    lea rax, [.reload]
    push rax
    retfq
.reload:
    mov eax, 0x10
    mov ss, ax
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret
