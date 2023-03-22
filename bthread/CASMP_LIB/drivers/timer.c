#include <timer.h>
#include <stdio.h>
/* to initialize timer */
void timer_init()
{
    timer_intr_disable(0); /* disable interrupt */
    timer_intr_disable(1);
    TIMER_INTR_CLEAR(0); /* clear all interrupt */
    TIMER_INTR_CLEAR(1);
    timer_module_disable(0); /* stop counting down */
    timer_module_disable(1);
    timer_set_mode(TIMER_MODE_FREE, 0); /* set to free running mode */
    timer_set_mode(TIMER_MODE_FREE, 1);
    timer_set_size(0, 0); /* set to 16-bits mode */
    timer_set_size(0, 1);
    timer_set_scale(TIMER_SCALE_1, 0); /* set the timing prescale to be 1 */
    timer_set_scale(TIMER_SCALE_1, 1);
    TIMER_LOAD_WRITE(0, 0); /* set to default value */
    TIMER_LOAD_WRITE(0, 1);
}

/* to enable the timer module to count down */
void timer_module_enable(unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;
    //printf("value before module_enable = %x\n",value);
    value |= TIMER_CTRL_MODULE_ENABLE;
    //printf("value after module_enable = %x\n",value);
    *(volatile unsigned int*)addr = value;
}

/* to disable the timer module to count down */
void timer_module_disable(unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;

    value &= ~TIMER_CTRL_MODULE_ENABLE;
    *(volatile unsigned int*)addr = value;
}

/* to enable the timer to assert interrupt */
void timer_intr_enable(unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;
    //printf("value before int_enable = %x\n",value);
    value |= TIMER_CTRL_INTR_ENABLE;
    //printf("value after int_enable = %x\n",value);
    *(volatile unsigned int*)addr = value;
}

/* to disable the timer to assert interrupt */
void timer_intr_disable(unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;
    value &= ~TIMER_CTRL_INTR_ENABLE;
    *(volatile unsigned int*)addr = value;
}

/*
 *  set the timer mode
 *
 *  mode = 0 => free running mode
 *  mode = 1 => period mode
 *  mode = 2 => one shot
 */
void timer_set_mode(unsigned char mode, unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;

    value &= ~(TIMER_CTRL_MODE | TIMER_CTRL_ONESHOT); /* clear mode & one shot bit */

    if((mode == TIMER_MODE_FREE) || (mode == TIMER_MODE_PERIOD))
    {
        value |= ((mode & 0x01) << 6);
    }

    else if(mode == TIMER_MODE_ONESHOT)
    {
        value |= TIMER_CTRL_ONESHOT;
    }

    *(volatile unsigned int*)addr = value;
}

/*
 *  set the timer size for 16/32-bits mode
 *
 *  size = 0 => 16-bits mode
 *  size = 1 => 32-bits mode
 */
void timer_set_size(unsigned char size, unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;

    value &= ~TIMER_CTRL_SIZE; /* clear the size bit */
    value |= ((size & 0x01) << 1);
    *(volatile unsigned int*)addr = value;
}

/*
 *  set the timer timing interval prescale
 *
 *  scale = 0 => clock is divided by 1
 *  scale = 1 => clock is divided by 16
 *  scale = 2 => clock is divided by 256
 */
void timer_set_scale(unsigned char scale, unsigned char num)
{
    unsigned int addr = (num & 0x01) ? TIMER_CONTROL_1 : TIMER_CONTROL_0;
    unsigned int value = *(volatile unsigned int*)addr;

    value &= ~TIMER_CTRL_SCALE; /* clear the prescale bits */
    value |= ((scale & 0x03) << 2);
    *(volatile unsigned int*)addr = value;
}

