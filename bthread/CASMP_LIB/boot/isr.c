/* --====================================================================--
 * File Name : isr.c
 * File Version : 1.0.0
 **-------------------------------------------------------------------------
 * Release Time : 2010/10/1 p.m. 04:03:09
 * Author       : Yuan-Ting Jiang <bill0822@gmail.com>
 * Note         : first release
 **-------------------------------------------------------------------------
 * Revision     : None
 **-------------------------------------------------------------------------
 * Release Information : NCKU EE CASLAB
 * ------------------------------------------------------------------------
 * Purpose             : Interrupt service routine
 * --====================================================================-- */
#include <isr.h>
#include <uart.h>
#include <stdio.h>
#include <peripherals.h>

//int x = 0;
//int y = 0;

/* system irq handler */
void IRQ_Handler(void)
{
    unsigned int num = *((unsigned int*)(VICIRQSTATUS));
    int i;

    //printf("Enter SYS IRQ, num = %d\n",num);

    for(i = 0; i < 31; i++)
    {
        if((num >> i) & 0x01)
        {
            switch(i)
            {
                    //case 1: SoftIntHandler()     ; break;
                case 4:
                    TimerAHB1IntHandler();
                    break;
                case 5:
                    TimerAHB2IntHandler();
                    break;
                    //case 6: TimerAHB1IntHandler(); break;
                    //case 7: TimerAHB2IntHandler(); break;
                default:
                    printf("Error interrupt number\n");
                    break;
            }
        }
    }

    //printf("Leave SYS IRQ, num = %d\n",num);
}

void DAB_Handler(void)
{
    while(1)
    {
        printf("Data abort Exception !!!\n");
    }
}

/* reset handler */
void RST_Handler(void)
{
    __asm__("mov pc, #0x0040");
}

void UND_Handler(void)
{
    while(1)
    {
        printf("Undefined Exception !!!\n");
    }
}

void SWI_Handler(void)
{
    printf("\nSWI : End of simulation !!!\n");
    uart_tx_char(0x04);
}

void PRE_Handler(void)
{
    while(1)
    {
        printf("Prefetch abort Exception !!!\n");
    }
}



void FIQ_Handler(void)
{
    while(1)
    {
        printf("FIQ Exception !!!\n");
    }
}

void TimerAHB1IntHandler(void)
{
    //printf("Timer_0\n");
    TIMER_INTR_CLEAR(0);
    vic_clear();
    //x++;
    //printf("X : %d\n", x);
}

void TimerAHB2IntHandler(void)
{
    //printf("Timer_1\n");
    TIMER_INTR_CLEAR(1);
    vic_clear();
    //y++;
    //printf("YYY : %d\n", y);
}