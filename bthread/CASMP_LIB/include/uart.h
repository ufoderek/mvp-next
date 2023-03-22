#include <io.h>
#include <mmio.h>

#ifndef _UART_H_
#define _UART_H_

/* MMIO parameters of PL011 UART controller */
#define UARTDR (MMIO_UART_0+(core_id<<12)+0x0000)
#define UARTRSR (MMIO_UART_0+(core_id<<12)+0x0004)
#define UARTFR (MMIO_UART_0+(core_id<<12)+0x0018)
#define UARTILPR (MMIO_UART_0+(core_id<<12)+0x0020)
#define UARTIBRD (MMIO_UART_0+(core_id<<12)+0x0024)
#define UARTFBRD (MMIO_UART_0+(core_id<<12)+0x0028)
#define UARTLCRH (MMIO_UART_0+(core_id<<12)+0x002c)
#define UARTCR (MMIO_UART_0+(core_id<<12)+0x0030)
#define UARTIFLS (MMIO_UART_0+(core_id<<12)+0x0034)
#define UARTIMSC (MMIO_UART_0+(core_id<<12)+0x0038)
#define UARTRIS (MMIO_UART_0+(core_id<<12)+0x003c)
#define UARTMIS (MMIO_UART_0+(core_id<<12)+0x0040)
#define UARTICR (MMIO_UART_0+(core_id<<12)+0x0044)
#define UARTDMACR (MMIO_UART_0+(core_id<<12)+0x0048)

/* UARTFR flag defined */
#define UART_FR_BUSY 0x08
#define UART_FR_RXFE 0x10
#define UART_FR_TXFF 0x20
#define UART_FR_RXFF 0x40
#define UART_FR_TXFE 0x80

/* operate parameters of PL011 UART controller */
#define UART_SPEED 38400 /* baud rate */

/* define macros */
#define UART_DISABLE()\
    IO_WRITE_HALF(UARTCR,0x0000) /* disable UART interface */

#define UART_ENABLE()\
    IO_WRITE_HALF(UARTCR,0x0301) /* turn on the UART to transfer and recieve */

/* function prototypings */
extern void uart_init(void);
extern inline void uart_wait_txff(void);
extern inline void uart_wait_rxfe(void);
extern void uart_tx_char(char);
extern int uart_tx_string(const char*);
extern char uart_rx_char(void);

/* CORE ID */
int core_id = 0;

#endif
