#include <vic.h>

/* to initialize VIC */
void vic_init(void)
{
    vic_mode(0x00000000);
    vic_enable(0x00000000);
}

/* to set the IRQ or FIQ mode for each channel */
void vic_mode(unsigned int channel)
{
    /* 1 for FIQ; 0 for IRQ */
    (*(unsigned int*)VICINTSELECT) = channel;
}

/* to enable or disable each interrupt channel */
void vic_enable(unsigned int channel)
{
    /* 1 for enable; 0 for disable */
    (*(unsigned int*)VICINTENABLE) = channel;
}

/* to set the vector interrupt enable or disable */
void vic_vector(unsigned short int source, unsigned short int vector, unsigned short int enable)
{
    (*(unsigned int*)(VICVECTCNTL0 + (vector << 2))) = ((enable & 0x0001) << 5) + (source & 0x001f);
}

/* to register the ISR address for vector interrupt */
void vic_vector_isr(unsigned int addr, unsigned short int vector)
{
    (*(unsigned int*)(VICVECTADDR0 + (vector << 2))) = addr;
}

/* clear vector interrupt signal */
void vic_clear()
{
    (*(unsigned int*)VICVECTADDR) = 0;
}
