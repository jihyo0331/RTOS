#include "stdint.h"
#include "stdbool.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "Uart.h"

#include "stdio.h"
#include "stdlib.h"

#include "Kernel.h"
#include "armcpu.h"

static void Hw_init(void);
static void Kernel_init(void);

static void Printf_test(void);
static void Timer_test(void);

static void Test_critical_section(uint32_t p, uint32_t taskId);

void User_task0(void);
void User_task1(void);
void User_task2(void);

extern volatile PL011_t* Uart;

static void Handle_shutdown(void)
{
    debug_printf("Shutdown requested. Halting...\n");
    disable_irq();
    while(true);
}

void main(void)
{
    Hw_init();

    uint32_t i = 100;
    while(i--)
    {
        Hal_uart_put_char('N');
    }
    Hal_uart_put_char('\n');

    putstr("Hello World!\n");

    Printf_test();
    Timer_test();

    Kernel_init();

    while(true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}


static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();
    Kernel_event_flag_init();
    Kernel_msgQ_init();
    Kernel_sem_init(1);
    Kernel_mutex_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task2 creation fail\n");
    }

    Kernel_start();
}

static void Printf_test(void)
{
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u\n", 0);
    debug_printf("SYSCTRL0 %x\n", *sysctrl0);
}

static void Timer_test(void)
{
    for(uint32_t i = 0; i < 5 ; i++)
    {
        debug_printf("current count : %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}

void User_task0(void)
{
    uint32_t local = 0;
    debug_printf("User Task #0 SP=0x%x\n", &local);

    uint8_t  cmdBuf[16];
    uint32_t cmdBufIdx = 0;
    uint8_t  uartch = 0;

    while(true)
    {
        // Poll UART in case interrupt delivery is lost
        if (Uart->uartfr.bits.RXFE == 0)
        {
            uint8_t ch = Hal_uart_get_char();

            if (ch == 'Q' || ch == 'q')
            {
                Kernel_send_events(KernelEventFlag_Shutdown);
            }
            else if (ch == 'U' || ch == 'u')
            {
                Kernel_send_events(KernelEventFlag_Unlock);
                Hal_uart_put_char(ch);
                debug_printf("[UART POLL] unlock key '%c'\n", ch);
                Kernel_yield();
                continue;
            }
            else if (ch == 'X' || ch == 'x')
            {
                Kernel_send_events(KernelEventFlag_CmdOut);
                Hal_uart_put_char(ch);
                debug_printf("[UART POLL] cmd key '%c'\n", ch);
                Kernel_yield();
                continue;
            }
            else
            {
                Hal_uart_put_char(ch);
                debug_printf("[UART POLL] char '%c'\n", ch);
                Kernel_send_msg(KernelMsgQ_Task0, &ch, 1);
                Kernel_send_events(KernelEventFlag_UartIn);
                Kernel_yield();
                continue;
            }
        }

        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_CmdOut|KernelEventFlag_Shutdown|KernelEventFlag_Tick);
        switch(handle_event)
        {
        case KernelEventFlag_UartIn:
            Kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);
            if (uartch == '\r')
            {
                cmdBuf[cmdBufIdx] = '\0';

                Kernel_send_msg(KernelMsgQ_Task1, &cmdBufIdx, 1);
                Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, cmdBufIdx);
                Kernel_send_events(KernelEventFlag_CmdIn);

                cmdBufIdx = 0;
            }
            else
            {
                cmdBuf[cmdBufIdx] = uartch;
                cmdBufIdx++;
                cmdBufIdx %= 16;
            }
            break;
        case KernelEventFlag_CmdOut:
            //debug_printf("\nCmdOut Event by Task0\n");
            Test_critical_section(5, 0);
            break;
        case KernelEventFlag_Shutdown:
            Handle_shutdown();
            break;
        case KernelEventFlag_Tick:
            // periodic tick: no-op, just yield
            break;
        }
        Kernel_yield();
    }
}

void User_task1(void)
{
    uint32_t local = 0;

    debug_printf("User Task #1 SP=0x%x\n", &local);

    uint8_t cmdlen = 0;
    uint8_t cmd[16] = {0};

    while(true)
    {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn|KernelEventFlag_Unlock|KernelEventFlag_Tick);
        switch(handle_event)
        {
        case KernelEventFlag_CmdIn:
            memclr(cmd, 16);
            Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
            Kernel_recv_msg(KernelMsgQ_Task1, cmd, cmdlen);
            debug_printf("\nRecv Cmd: %s\n", cmd);
            break;
        case KernelEventFlag_Unlock:

            break;
        case KernelEventFlag_Tick:
            // periodic tick: no-op, just yield
            break;
        }
        Kernel_yield();
    }
}

void User_task2(void)
{
    uint32_t local = 0;

    debug_printf("User Task #2 SP=0x%x\n", &local);

    while(true)
    {
        Test_critical_section(3, 2);
        Kernel_yield();
    }
}

static uint32_t shared_value;
static void Test_critical_section(uint32_t p, uint32_t taskId)
{
    Kernel_lock_mutex();

    debug_printf("User Task #%u Send=%u\n", taskId, p);
    shared_value = p;
    Kernel_yield();
    delay(1000);
    debug_printf("User Task #%u Shared Value=%u\n", taskId, shared_value);

    Kernel_unlock_mutex();
}
