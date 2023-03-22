#ifndef _STDARG_H_
#define _STDARG_H_

typedef __builtin_va_list va_list;

/* the start address of first variant argument */
#define va_start(AP,LASTARG)\
    __builtin_va_start(AP,LASTARG)

/* end of the variant argument */
#define va_end(AP)\
    __builtin_va_end(AP)

/* to access next argument */
#define va_arg(AP,TYPE)\
    __builtin_va_arg(AP,TYPE)

#endif
