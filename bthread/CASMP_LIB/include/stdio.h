#include <stdarg.h>

#ifndef _STDIO_H_
#define _STDIO_H_

#define NULL 0
#define TRUE 1
#define FALSE 0

#define _INF 0
#define _NAN 1
#define _DENORMAL 2
#define _NORMAL 3
#define _BIGF 4

extern int printf(const char*, ...);
extern int vprintf(const char*, va_list);
extern int kdoprint(void (*)(char), const char*, va_list);

extern inline void putchar(const char);
extern inline int puts(const char*);
extern inline char getchar(void);

#endif
