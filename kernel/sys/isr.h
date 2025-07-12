#ifndef ISR_H
#define ISR_H

#include <types.h>

// very cool macro taken from https://github.com/NSG650/Polaris/blob/cce2d98a3deb0e7ed88ed8d6c288ad53efc0d611/kernel/arch/x86_64-pc/include/sys/isr.h#L4

#define PASTER(x, y) x##y
#define EVALUATOR(x, y) PASTER(x, y)
// This first macro makes functions like "void isr0(void)"
// But the number is summed up on each call
#define ONE EVALUATOR(void isr, __COUNTER__)(void)
// Different granularities
#define TWO \
	ONE;    \
	ONE
#define TEN \
	TWO;    \
	TWO;    \
	TWO;    \
	TWO;    \
	TWO
#define TWENTY \
	TEN;       \
	TEN
#define HUNDRED \
	TWENTY;     \
	TWENTY;     \
	TWENTY;     \
	TWENTY;     \
	TWENTY
// Define 255 ISRs based on previous granularities
#define DEFISR \
	HUNDRED;   \
	HUNDRED;   \
	TWENTY;    \
	TWENTY;    \
	TEN;       \
	TWO;       \
	TWO;       \
	TWO

DEFISR;
typedef struct registers_t {
	u64 r15;
	u64 r14;
	u64 r13;
	u64 r12;
	u64 r11;
	u64 r10;
	u64 r9;
	u64 r8;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 rdx;
	u64 rcx;
	u64 rbx;
	u64 rax;
	u64 core;
	u64 isr_number;
	u64 err_code;
	u64 rip;
	u64 cs;
	u64 rflags;
	u64 rsp;
	u64 ss;
} __attribute__((packed)) registers_t;

typedef void (*event_handlers_t)(registers_t *);

void isr_init(void);

extern void isr_handle(registers_t* r);
void isr_set_handler(int n, void* handler);

#endif
