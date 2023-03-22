#include <io.h>
#include <uart.h>

/****************************************************************************************************
* to initialize the ARM PL011 UART controller
* baud rate    => 38400
* bit width    => 8
* stop bit     => 1
* parity       => no
* bit stream   => FIFO
*
* parameters:
*  none
* return:
*  none
****************************************************************************************************/
void uart_init()
{

    /* get core id*/
    asm volatile("mrc p15, 0, %0, c0, c0, 5\n" : "=r"(core_id));

    if(core_id == 0)
    {
        /* diable the UART interface */
        UART_DISABLE();

        /* set baud rate to be 38400 and UART_CLK to be 24MHz */
        /* The setup values are calculated by ourself instead of CPU */
        /* Be careful of the setup values */
        IO_WRITE_HALF(UARTIBRD, 39); /* IBRD = UART_CLK / (16 * baud rate) */
        IO_WRITE_HALF(UARTFBRD, 4); /* FBRD = round(64 * (UART_CLK % (16 * baud rate)) / (16 * baud rate)) */

        /* set to 8-bits mode, 1 stop bit, no parity, FIFO enable */
        IO_WRITE_BYTE(UARTLCRH, 0x70);

        /* enable the UART interface */
        UART_ENABLE();
        setting = 1;
    }

    else
    {
        while(setting == 0)
        {
            ;
        }
    }
}

/****************************************************************************************************
* detect whether the transfer buffer is full or not
* system idle until the buffer is not full
*
* parameters:
*  none
* return:
*  none
****************************************************************************************************/
inline void uart_wait_txff()
{
    /* get core id*/
    //asm volatile("mrc p15, 0, %0, c0, c0, 5\n" : "=r" (core_id));

    unsigned short int flag;

    do
    {
        IO_READ_HALF(UARTFR, flag);
    }
    while(flag & UART_FR_TXFF);
}

/****************************************************************************************************
* detect whether the receive buffer is empty or not
* system idle until the buffer is not empth
*
* parameters:
*  none
* return:
*  none
****************************************************************************************************/
inline void uart_wait_rxfe()
{
    /* get core id*/
    //asm volatile("mrc p15, 0, %0, c0, c0, 5\n" : "=r" (core_id));

    char flag;

    do
    {
        IO_READ_HALF(UARTFR, flag);
    }
    while(flag & UART_FR_RXFE);
}

/****************************************************************************************************
* transfer and send a byte data to UART interface
*
* parameters:
*  data    => character data to be transfered out
* return:
*  none
****************************************************************************************************/
void uart_tx_char(char data)
{
    /* get core id*/
    //asm volatile("mrc p15, 0, %0, c0, c0, 5\n" : "=r" (core_id));

    if(data == '\n')
    {
        uart_wait_txff();
        IO_WRITE_BYTE(UARTDR, '\r');
        uart_wait_txff();
        IO_WRITE_BYTE(UARTDR, data);
    }

    else
    {
        uart_wait_txff();
        IO_WRITE_BYTE(UARTDR, data);
    }
}

/****************************************************************************************************
* transfer and send a string data to UART interface
*
* parameters:
*  str => string data to be transfered out
* return:
*  none
****************************************************************************************************/
int uart_tx_string(const char* str)
{
    char data;
    int count = 0;

    while((data = *str) != '\0')
    {
        uart_tx_char(data);
        str++;
        count++;
    }

    return count;
}

/****************************************************************************************************
* recieve a byte data from UART interface
*
* parameters:
*  none
* return:
*  the data recieving
****************************************************************************************************/
char uart_rx_char()
{
    /* get core id*/
    //asm volatile("mrc p15, 0, %0, c0, c0, 5\n" : "=r" (core_id));

    char data;

    uart_wait_rxfe();
    IO_READ_BYTE(UARTDR, data);
    return data;
}
