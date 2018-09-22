#ifndef __STRING_H__
#define __STRING_H__
#include "typedef.h"

size_t strlen(const char *s);
void *memcpy(void *dest,void *src, size_t n);
char* strcpy(char* s1, const char* s2);
int strncpy(char *s1,char *s2,size_t count);
int strcmp (const char* s1, const char* s2);
int strncmp (const char* s1,const char* s2,size_t count);
void* memset(void *s, char val, size_t count);
void* memsetw(void *s, short val, size_t count);

#endif
