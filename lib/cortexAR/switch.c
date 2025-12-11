#include "stdint.h"
#include "stdbool.h"
#include "armcpu.h"
#include "switch.h"
#include "task.h"

extern KernelTcb_t* gCurrent_tcb;
extern KernelTcb_t* gNext_tcb;

// Fault injection: deliberately unbalance the restore sequence to corrupt LR/PC
// and crash during/after a context switch.
#define INJECT_CONTEXT_POP_MISMATCH 1

__attribute__ ((naked)) void Arch_start(void)
{
	__asm__ volatile ("B Arch_Restore_context");
}

__attribute__ ((naked)) void Arch_context_switching(void)
{
    __asm__ ("B Arch_Save_context");
    __asm__ ("B Arch_Restore_context");
}

__attribute__ ((naked)) void Arch_Save_context(void)
{
    // save current task context into the current task stack
    __asm__ ("PUSH {lr}");
    __asm__ ("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("MRS   r0, cpsr");
    __asm__ ("PUSH {r0}");
    // save current task stack pointer into the current TCB
    __asm__ ("LDR   r0, =gCurrent_tcb");
    __asm__ ("LDR   r0, [r0]");
    __asm__ ("STMIA r0!, {sp}");
}

__attribute__ ((naked)) void Arch_Restore_context(void)
{
    // restore next task stack pointer from the next TCB
    __asm__ ("LDR   r0, =gNext_tcb");
    __asm__ ("LDR   r0, [r0]");
    __asm__ ("LDMIA r0!, {sp}");
    // restore next task context from the next task stack
    __asm__ ("POP  {r0}");
    __asm__ ("MSR   cpsr, r0");
#if INJECT_CONTEXT_POP_MISMATCH
    // Pop one fewer register so the final PC is loaded from the wrong slot.
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11}");
#else
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
#endif
    __asm__ ("POP  {pc}");
}
