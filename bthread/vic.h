#ifndef _VIC_H_
#define _VIC_H_

/* base address of the VIC interrupt controller */
#define BASE_VIC 0x10140000

/* memory mapping address of all control registers on VIC */
#define VICIRQSTATUS (BASE_VIC+0x0000)
#define VICFIQSTATUS (BASE_VIC+0x0004)
#define VICRAWINTR (BASE_VIC+0x0008)
#define VICINTSELECT (BASE_VIC+0x000c)
#define VICINTENABLE (BASE_VIC+0x0010)
#define VICINTENCLEAR (BASE_VIC+0x0014)
#define VICSOFTINT (BASE_VIC+0x0018)
#define VICSOFTINTCLEAR (BASE_VIC+0x001c)
#define VICPROTECTION (BASE_VIC+0x0020)
#define VICVECTADDR (BASE_VIC+0x0030)
#define VICDEFVECTADDR (BASE_VIC+0x0034)
#define VICVECTADDR0 (BASE_VIC+0x0100)
#define VICVECTADDR1 (BASE_VIC+0x0104)
#define VICVECTADDR2 (BASE_VIC+0x0108)
#define VICVECTADDR3 (BASE_VIC+0x010c)
#define VICVECTADDR4 (BASE_VIC+0x0110)
#define VICVECTADDR5 (BASE_VIC+0x0114)
#define VICVECTADDR6 (BASE_VIC+0x0118)
#define VICVECTADDR7 (BASE_VIC+0x011c)
#define VICVECTADDR8 (BASE_VIC+0x0120)
#define VICVECTADDR9 (BASE_VIC+0x0124)
#define VICVECTADDR10 (BASE_VIC+0x0128)
#define VICVECTADDR11 (BASE_VIC+0x012c)
#define VICVECTADDR12 (BASE_VIC+0x0130)
#define VICVECTADDR13 (BASE_VIC+0x0134)
#define VICVECTADDR14 (BASE_VIC+0x0138)
#define VICVECTADDR15 (BASE_VIC+0x013c)
#define VICVECTCNTL0 (BASE_VIC+0x0200)
#define VICVECTCNTL1 (BASE_VIC+0x0204)
#define VICVECTCNTL2 (BASE_VIC+0x0208)
#define VICVECTCNTL3 (BASE_VIC+0x020c)
#define VICVECTCNTL4 (BASE_VIC+0x0210)
#define VICVECTCNTL5 (BASE_VIC+0x0214)
#define VICVECTCNTL6 (BASE_VIC+0x0218)
#define VICVECTCNTL7 (BASE_VIC+0x021c)
#define VICVECTCNTL8 (BASE_VIC+0x0220)
#define VICVECTCNTL9 (BASE_VIC+0x0224)
#define VICVECTCNTL10 (BASE_VIC+0x0228)
#define VICVECTCNTL11 (BASE_VIC+0x022c)
#define VICVECTCNTL12 (BASE_VIC+0x0230)
#define VICVECTCNTL13 (BASE_VIC+0x0234)
#define VICVECTCNTL14 (BASE_VIC+0x0238)
#define VICVECTCNTL15 (BASE_VIC+0x023c)

/* function prototyping of VIC */
extern void vic_init(void);
extern void vic_mode(unsigned int);
extern void vic_enable(unsigned int);
extern void vic_vector(unsigned short int, unsigned short int, unsigned short int);
extern void vic_vector_isr(unsigned int, unsigned short int);
extern void vic_clear(void);

#endif
