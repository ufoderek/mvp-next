#ifndef _TIMER_H_
#define _TIMER_H_

/* base address of timmer */
#define BASE_TIMER 0x101e2000

/* memory mapping address of all control registers of timmer */
#define TIMER_LOAD_0 (BASE_TIMER+0x0000)
#define TIMER_VALUE_0 (BASE_TIMER+0x0004)
#define TIMER_CONTROL_0 (BASE_TIMER+0x0008)
#define TIMER_INTCLR_0 (BASE_TIMER+0x000c)
#define TIMER_RIS_0 (BASE_TIMER+0x0010)
#define TIMER_MIS_0 (BASE_TIMER+0x0014)
#define TIMER_BGL_0 (BASE_TIMER+0x0018)
#define TIMER_LOAD_1 (BASE_TIMER+0x0020)
#define TIMER_VALUE_1 (BASE_TIMER+0x0024)
#define TIMER_CONTROL_1 (BASE_TIMER+0x0028)
#define TIMER_INTCLR_1 (BASE_TIMER+0x002c)
#define TIMER_RIS_1 (BASE_TIMER+0x0030)
#define TIMER_MIS_1 (BASE_TIMER+0x0034)
#define TIMER_BGL_1 (BASE_TIMER+0x0038)
#define TIMER_ITCR (BASE_TIMER+0x0f00)
#define TIMER_ITOP (BASE_TIMER+0x0f04)
#define TIMER_PID_0 (BASE_TIMER+0x0fe0)
#define TIMER_PID_1 (BASE_TIMER+0x0fe4)
#define TIMER_PID_2 (BASE_TIMER+0x0fe8)
#define TIMER_PID_3 (BASE_TIMER+0x0fec)
#define TIMER_CID_0 (BASE_TIMER+0x0ff0)
#define TIMER_CID_1 (BASE_TIMER+0x0ff4)
#define TIMER_CID_2 (BASE_TIMER+0x0ff8)
#define TIMER_CID_3 (BASE_TIMER+0x0ffc)

/* status for TIMER_CONTROL */
#define TIMER_CTRL_MODULE_ENABLE 0x80
#define TIMER_CTRL_MODE 0x40
#define TIMER_CTRL_INTR_ENABLE 0x20
#define TIMER_CTRL_SCALE 0x0c
#define TIMER_CTRL_SIZE 0x02
#define TIMER_CTRL_ONESHOT 0x01

/* working mode of the timer */
enum MODE
{
    TIMER_MODE_FREE,
    TIMER_MODE_PERIOD,
    TIMER_MODE_ONESHOT
};

/* timing prescale parameter */
enum SCALING
{
    TIMER_SCALE_1,
    TIMER_SCALE_4,
    TIMER_SCALE_8
};

/* load value into TIMER_LOAD register */
#define TIMER_LOAD_WRITE(value,x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_LOAD_1=value):\
    (*(volatile unsigned int*)TIMER_LOAD_0=value)

/*
 * load value into TIMER_BGL register
 *
 * the difference between TIMER_LOAD & TIMER_BGL is that
 * writing to TIMER_BGL does't cause the TIMER_VALUE be reset immediately
 */
#define TIMER_BGL_WIRTE(value,x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_BGL_1=value):\
    (*(volatile unsigned int*)TIMER_BGL_0=value)

/* read current count value */
#define TIMER_CURRENT_VALUE(x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_VALUE_1):\
    (*(volatile unsigned int*)TIMER_VALUE_0)

/* read current load value */
#define TIMER_CURRENT_LOAD(x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_LOAD_1):\
    (*(volatile unsigned int*)TIMER_LOAD_0)

/* read current load value */
#define TIMER_CURRENT_LOAD(x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_LOAD_1):\
    (*(volatile unsigned int*)TIMER_LOAD_0)

/* read current control value */
#define TIMER_CURRENT_CNTL(x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_CONTROL_1):\
    (*(volatile unsigned int*)TIMER_CONTROL_0)

/* clear the interrupt */
#define TIMER_INTR_CLEAR(x)\
    (x&0x01)?\
    (*(volatile unsigned int*)TIMER_INTCLR_1=0):\
    (*(volatile unsigned int*)TIMER_INTCLR_0=0)

/* function prototyping of timer */
extern void timer_init(void);
extern void timer_module_enable(unsigned char);
extern void timer_module_disable(unsigned char);
extern void timer_intr_enable(unsigned char);
extern void timer_intr_disable(unsigned char);
extern void timer_set_mode(unsigned char, unsigned char);
extern void timer_set_size(unsigned char, unsigned char);
extern void timer_set_scale(unsigned char, unsigned char);

#endif
