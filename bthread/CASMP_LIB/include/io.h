#ifndef _IO_H_
#define _IO_H_

#define IO_READ_BYTE(addr,data)\
    data=(*(volatile unsigned char*)(addr))

#define IO_READ_HALF(addr,data)\
    data=(*(volatile unsigned short int*)(addr))

#define IO_READ_WORD(addr,data)\
    data=(*(volatile unsigned int*)(addr))

#define IO_READ_DOUBLE(addr,data)\
    data=(*(volatile unsigned long long int*)(addr))

#define IO_WRITE_BYTE(addr,data)\
    (*(volatile unsigned char*)(addr))=(data)

#define IO_WRITE_HALF(addr,data)\
    (*(volatile unsigned short int*)(addr))=(data)

#define IO_WRITE_WORD(addr,data)\
    (*(volatile unsigned int*)(addr))=(data)

#define IO_WRITE_DOUBLE(addr,data)\
    (*(volatile unsigned long long int*)(addr))=(data)

#endif
