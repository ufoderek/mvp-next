#include <string.h>

/****************************************************************************************************
* to copy data from source address to destination address
*
* parameter:
*  *dest   => the destination address
*  *src    => the source address
*  length  => length to copy(in bytes)
* return:
*  the destination address
****************************************************************************************************/
void* memcpy(void* dest, const void* src, unsigned int length)
{
    char* tmp_dest, *tmp_src;

    tmp_dest = (char*)dest;
    tmp_src = (char*)src;

    while(length)
    {
        *tmp_dest = *tmp_src;
        tmp_dest++;
        tmp_src++;
        length--;
    }

    return dest;
}



/****************************************************************************************************
* force to set a memory space to store a defined data
*
* parameter:
*  *addr   => the memory address
*  data    => data to be set
*  length  => length to set data(in bytes)
* return:
*  the memory address
****************************************************************************************************/
void* memset(const void* addr, char data, unsigned int length)
{
    char* tmp;

    tmp = (char*)addr;

    while(length)
    {
        *tmp = data;
        tmp++;
        length--;
    }

    return (void*)addr;
}

/****************************************************************************************************
* to compare two data streams
*
* parameter:
*  *data_1 => the data stream 1
*  *data_2 => the data stream 2
*  length  => length to compare(in bytes)
* return:
*  the compare result(0 for not equal; 1 for equal)
****************************************************************************************************/
int memcmp(const void* data_1, const void* data_2, unsigned int length)
{
    const char* tmp_1, *tmp_2;
    int result = 1;

    tmp_1 = (const char*)data_1;
    tmp_2 = (const char*)data_2;

    while(length)
    {
        if(*tmp_1 == *tmp_2)
        {
            tmp_1++;
            tmp_2++;
            length--;
        }

        else
        {
            result = 0;
            break;
        }
    }

    return result;
}

/****************************************************************************************************
* to compare two strings
*
* parameter:
*  *str_1  => input string 1
*  *str_2  => input string 2
* return:
*  the compare result(0 for not equal; 1 for equal)
****************************************************************************************************/
int strcmp(const char* str_1, const char* str_2)
{
    int result = 1;
    unsigned int length;

    if((length = strlen(str_1)) != strlen(str_2))   /* these two strings' length are not equal */
    {
        result = 0;
    }

    else
    {
        while(length)
        {
            if(*str_1 != *str_2)
            {
                result = 0;
                break;
            }

            else
            {
                str_1++;
                str_2++;
                length--;
            }
        }
    }

    return result;
}

/****************************************************************************************************
* to calculate the lngth of input string
*
* parameter:
*  *str    => input string
* return:
*  length of the input string
****************************************************************************************************/
unsigned int strlen(const char* str)
{
    unsigned int count = 0;

    while(*str != '\0')
    {
        str++;
        count++;
    }

    return count;
}

/****************************************************************************************************
* to copy a string
*
* parameter:
*  *dest   => start address of destination string
*  *src    => start address of source string
* return:
*  the destination address
****************************************************************************************************/
char* strcpy(char* dest, const char* src)
{
    char* tmp_dest;
    const char* tmp_src;

    tmp_dest = dest;
    tmp_src = src;

    /* copy */
    while(*tmp_src != '\0')
    {
        *tmp_dest = *tmp_src;
        tmp_dest++;
        tmp_src++;
    }

    *tmp_dest = '\0'; /* adding the tail flag */
    return dest;
}

/****************************************************************************************************
* capture and combine the source string to destination string
*
* parameter:
*  *dest   => start address of destination string
*  *src    => start address of source string
* return:
*  the destination address
****************************************************************************************************/
char* strcat(char* dest, const char* src)
{
    char* tmp_dest;
    const char* tmp_src;

    tmp_dest = dest;
    tmp_src = src;

    /* move the pointer to the tail of destination string */
    while(*tmp_dest != '\0')
    {
        tmp_dest++;
    }

    strcpy(tmp_dest, tmp_src); /* string copy and combine */
    return dest;
}
