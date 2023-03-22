#ifndef _STRING_H_
#define _STRING_H_

void* memcpy(void*, const void*, unsigned int);
void* memset(const void*, char, unsigned int);
int memcmp(const void*, const void*, unsigned int);
int strcmp(const char*, const char*);
unsigned int strlen(const char*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);

#endif
