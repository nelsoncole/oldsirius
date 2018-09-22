#include <string.h>
size_t strlen(const char *s)
{
	char *tmp = (char*)s;
	while(*tmp != '\0')
	tmp++;
	return (size_t)(tmp - s);
}

void *memcpy(void *dest,void *src, size_t n)
{
	size_t p    = n;
	char *p_dest = (char*)dest;
	char *p_src  = (char*)src;

	while(p--)
	*p_dest++ = *p_src++;
	return dest;
}

char* strcpy(char* s1, const char* s2)
{
    char *p = (char*)s1;
    int i =0;
    while (s2[i] != '\0'){
    s1[i] = s2[i]; i++;
    }
    return p;
}

int strncpy(char *s1,char *s2,size_t count)
{
    memcpy(s1,s2,count);
    return count;
}

int strcmp (const char* s1, const char* s2)
{

    while((*s1++ == *s2++))
    if((*s1 == *s2) && (*s1 + *s2 == 0))
    return 0;


    return -1;
}
int strncmp (const char* s1,const char* s2,size_t count)
{
	

	while(--count)
	if(*s1++ != *s2++)
	break;

	return (count);
}

void* memset(void *s, char val, size_t count)
{
        uint8_t *tmp = (uint8_t *)s;
        for(; count != 0; count--) *tmp++ = val;
        return s;    	
}

void* memsetw(void *s, short val, size_t count)
{	
        uint16_t *tmp = (uint16_t *)s;
        for( ;count != 0; count--) *tmp++ = val;
        return s;    	
}


