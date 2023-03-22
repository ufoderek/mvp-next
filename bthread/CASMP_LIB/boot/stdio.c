#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <uart.h>

/****************************************************************************************************
* the C standard "printf" function
*
* parameter:
*  *fmt    => string to output
* return:
*  total characters outputing
****************************************************************************************************/
int printf(const char* fmt, ...)
{
    va_list ap;
    int count;

    va_start(ap, fmt);
    count = vprintf(fmt, ap);
    va_end(ap);
    return count;
}

static int kprintn(void (*put)(char), unsigned long value, int base)
{
    char* ptr;
    char buff[sizeof(long) * 8 / 3 + 1];
    int count = 0;

    ptr = buff;

    do
    {
        *ptr++ = "0123456789abcdef"[value % base];
        value /= base;
    }
    while(value);

    do
    {
        put(*--ptr);
        count++;
    }
    while(ptr > buff);

    return count;
}

double powerof2_n(unsigned long long fraction, int power)
{
    int i;

    double tmp = 1.00;
    double fraction_d = fraction / tmp;

    for(i = 0; i < power; i++)
    {
        fraction_d = fraction_d / 2;
    }

    return fraction_d;
}

double round_of(int power)
{
    double tmp[10] = {0.5,
                      0.05,
                      0.005,
                      0.0005,
                      0.00005,
                      0.000005,
                      0.0000005,
                      0.00000005,
                      0.000000005,
                      0.0000000005
                     };

    if((power > 9) || (power < 0))
    {
        return 0.0;
    }

    else
    {
        return tmp[power];
    }
}

static int p_ieee754(void (*put)(char), unsigned long long fraction, int power, int precise, unsigned int type)
{
    int count = 0;
    int i;
    char num;

    switch(type)
    {
        case _INF:
            put('I');
            put('n');
            put('f');
            count += 3;
            break;

        case _NAN:
            put('N');
            put('a');
            put('N');
            count += 3;
            break;

        case _BIGF:
            put('B');
            put('i');
            put('g');
            count += 3;
            break;

        case _DENORMAL:
            put('.');

            for(i = 0; i < precise; i++)
            {
                put('0');
                count++;
            }

            break;

        case _NORMAL:
            put('.');
            double fraction_d;
            fraction_d = powerof2_n(fraction, power);    //+ round_of(precise);

            if((fraction_d + round_of(precise)) < 1.0)
            {
                fraction_d = fraction_d + round_of(precise);
            }

            for(i = 0; i < precise; i++)
            {
                fraction_d = fraction_d * 10;
                num = "0123456789"[(int)fraction_d % 10];
                put(num);
                count++;
            }

            break;

        default:
            break;
    }

    return count;
}

static int kprintn_d(void (*put)(char), double value, int precise)
{
    int i;
    int count = 0;
    unsigned int* tmp = (unsigned int*)&value;

    unsigned int index[2];

    for(i = 0; i < 2; i++)
    {
        index[1 - i] = *tmp++;
    }

    int exp = ((index[0] >> 20) & 0x000007FF);
    int t_exp = exp - 1023;
    unsigned int fraction_1 = index[0] & 0x000FFFFF;
    unsigned int fraction_2 = index[1];

    unsigned long long fraction;

    if(t_exp < 0)
    {
        fraction = ((unsigned long long)0x01 << 52) + ((unsigned long long)fraction_1 << 32) + fraction_2;
    }

    else
    {
        fraction = ((unsigned long long)fraction_1 << 32) + fraction_2;
    }

    if(exp == 0x00)
    {
        if(fraction == 0x0)
        {
            count += kprintn(put, (unsigned long)0, 10); //Zero
        }

        else
        {
            count += kprintn(put, (unsigned long)0, 10);
            count += p_ieee754(put, fraction, 0, precise, _DENORMAL); //Denormalized
        }
    }

    else if(exp == 0x7FF)
    {
        if(fraction == 0x0)
        {
            count += p_ieee754(put, fraction, 0, 0, _INF); //Infinity
        }

        else
        {
            count += p_ieee754(put, fraction, 0, 0, _NAN); //Not a number
        }
    }

    else
    {
        if(t_exp > 52)
        {
            count += p_ieee754(put, fraction, 0, 0, _BIGF); //Big float
        }

        else
        {
            count += kprintn(put, (unsigned long)value, 10);
            count += p_ieee754(put, fraction, 52 - t_exp, precise, _NORMAL); //Normalized
        }
    }

    return count;
}

int vprintf(const char* fmt, va_list ap)
{
    return kdoprint(putchar, fmt, ap);
}

int kdoprint(void (*put)(char), const char* fmt, va_list ap)
{
    int count = 0;
    int end = 0;
    int lflag = 0;
    int f_precise;
    int c;
    double c_double;
    unsigned long value;

    while(!end)
    {
        /* get the next char */
        c = *fmt;
        fmt++;

        switch(c)
        {
                /* end of printing */
            case '\0':
                end = 1;
                break;

                /* special printing command */
            case '%':
_again:
                /* get the command parameter */
                c = *fmt;
                fmt++;

                switch(c)
                {
                    case '.':
                        c = *fmt;
                        fmt++;

                        if((c <= '9') && (c >= '0'))            //%.[0-9]f
                        {
                            f_precise = c - '0';
                            c = *fmt;
                            fmt++;
                        }

                        else if(c == 'f')            //%.f
                        {
                            f_precise = 5;         //default .xxxxx
                        }

                        else
                        {
                            printf("float passing error\n");
                        }

                        c_double = va_arg(ap, double);

                        if(c_double < 0)
                        {
                            put('-');
                            c_double = -c_double;
                            count++;
                        }

                        count += kprintn_d(put, c_double, f_precise);
                        break;
                    case '0'-'9':
                        goto _again;            /* ignore this flag */
                        break;
                    case '%':
                        put(c);
                        count++;
                        break;
                    case 'c':
                        c = va_arg(ap, int);
                        put(c & 0x00ff);
                        count++;
                        break;

                    case 'f':
                        c_double = va_arg(ap, double);

                        if(c_double < 0)
                        {
                            put('-');
                            c_double = -c_double;
                            count++;
                        }

                        count += kprintn_d(put, c_double, 5);
                        break;

                    case 'd':
                    case 'i':
                        value = (lflag) ?
                                va_arg(ap, long) :
                                va_arg(ap, int);

                        if((long)value < 0)
                        {
                            put('-');
                            value = -(long)value;
                            count++;
                        }

                        count += kprintn(put, value, 10);
                        lflag = 0;
                        break;
                    case 'h':
                    case 'H':
                        /* ignore this flag */
                        goto _again;
                        break;
                    case 'l':
                    case 'L':
                        lflag = 1;
                        goto _again;
                        break;
                    case 'o':
                        value = (lflag) ?
                                va_arg(ap, unsigned long) :
                                va_arg(ap, unsigned int);
                        count += kprintn(put, value, 8);
                        lflag = 0;
                        break;
                    case 'p':
                        value = va_arg(ap, unsigned int);
                        puts("0x");
                        count = count + kprintn(put, value, 16) + 2;
                        lflag = 0;
                        break;
                    case 's':
                        c = va_arg(ap, char*);
                        count += puts((char*)c);
                        break;
                    case 'u':
                        value = (lflag) ?
                                va_arg(ap, unsigned long) :
                                va_arg(ap, unsigned int);
                        count += kprintn(put, value, 10);
                        lflag = 0;
                        break;
                    case 'x':
                    case 'X':
                        value = (lflag) ?
                                va_arg(ap, unsigned long) :
                                va_arg(ap, unsigned int);
                        count += kprintn(put, value, 16);
                        lflag = 0;
                        break;
                    default:
                        break;
                }

                break;

                /* normal output */
            default:
                put(c);
                count++;
                break;
        }
    }

    va_end(ap);
    return count;
}

/****************************************************************************************************
* the C standard "putchar" function
*
* parameters:
*  data    => data to output
* return:
*  none
****************************************************************************************************/
inline void putchar(const char data)
{
    uart_tx_char(data);
}

/****************************************************************************************************
* the C standard "puts" function
*
* parameters:
*  *str    => string to output
* return:
*  total characters outputing
****************************************************************************************************/
inline int puts(const char* str)
{
    return uart_tx_string(str);
}

/****************************************************************************************************
* the C standard "getchar" function
*
* parameters:
*  none
* return:
*  the data input
****************************************************************************************************/
inline char getchar()
{
    return uart_rx_char();
}
