/*
 * task.c
 *
 *  Created on: Oct 22, 2018
 *      Author: maanu
 */


#include "stdint.h"
#include "stdbool.h"

#include "PSR.h"
#include "switch.h"

#include "task.h"

#include "MemoryMap.h"

#define MAX_TASK_NUM            (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)

KernelTcb_t* gCurrent_tcb;
KernelTcb_t* gNext_tcb;

static KernelTcb_t  sTask_list[MAX_TASK_NUM];
static uint32_t     sAllocated_tcb_index;
static uint32_t     sCurrent_tcb_index;

static KernelTcb_t* Scheduler_round_robin_algorithm(void);

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;

    for(uint32_t i = 0 ; i < MAX_TASK_NUM ; i++)
    {
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE;
    }
}

void Kernel_task_start(void)
{
    gNext_tcb = &sTask_list[sCurrent_tcb_index];
    Arch_start();
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return NOT_ENOUGH_TASK_NUM;
    }

    // Build initial stack frame matching Arch_Restore_context pop order:
    // [spsr][r0..r12][pc]
    uint32_t* sp = (uint32_t*)new_tcb->sp;
    sp -= (1 /*spsr*/ + 13 /*r0-r12*/ + 1 /*pc*/);

    sp[0] = PSR_INIT;            // will be popped into r0 then written to cpsr
    for (uint32_t i = 0; i < 13; i++)
    {
        sp[1 + i] = 0;           // r0-r12
    }
    sp[14] = (uint32_t)startFunc; // pc

    new_tcb->sp = (uint32_t)sp;

    return (sAllocated_tcb_index - 1);
}

uint32_t Kernel_task_get_current_task_id(void)
{
    return sCurrent_tcb_index;
}

void Kernel_task_scheduler(void)
{
    gCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    gNext_tcb = Scheduler_round_robin_algorithm();

    Arch_context_switching();
}

static KernelTcb_t* Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}
