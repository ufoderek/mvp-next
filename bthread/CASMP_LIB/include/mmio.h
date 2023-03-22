#ifndef _MMIO_H_
#define _MMIO_H_

/* the MMIO base address for each peripherals on ARM Versatilepb platform */
#define MMIO_FPGA 0x80000000 /* Real View logic tile Xilinx FPGA */
#define MMIO_LCD 0x10008000 /* character LCD */
#define MMIO_CLCD 0x10120000 /* Hitach PL110 CLCD */

#define MMIO_TIMER_0 0x10011000 /* SP804 timer 0 */
#define MMIO_TIMER_1 0x10012000 /* SP804 timer 1 */

#define MMIO_UART_0 0x10009000 /* PL011 UART 0 */
#define MMIO_UART_1 0x1000a000 /* PL011 UART 1 */
#define MMIO_UART_2 0x1000b000 /* PL011 UART 2 */
#define MMIO_UART_3 0x1000c000 /* PL011 UART 3 */

#define MMIO_GIC_0 0x10040000 /* PL190 VIC0(PIC) */

#define MMIO_PIC 0x10140000 /* PL190 VIC0(PIC) */
#define MMIO_SIC 0x10003000 /* PL190 VIC1(SIC) */

#endif
