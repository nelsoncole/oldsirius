#ifndef __STRING_H__
#define __STRING_H__

#include "ctype.h"

size_t strlen(const char *s);
void *memcpy(void *dest,void *src, size_t n);
char* strcpy(char* s1, const char* s2);
int strncpy(char *s1,const char *s2,size_t count);
int strcmp (char* s1, const char* s2);
int strncmp (char* s1,const char* s2,size_t count);
void* memset(void *s, char val, size_t count);
void* memsetw(void *s, short val, size_t count);
char *strcat(char *dest, const char *src);
char *strtok(char *dest, char *src,char control);

#endif
