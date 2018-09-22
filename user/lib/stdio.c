#include <asm.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/*Nelson Cole.
 *
 *
 *  Simples E/S
 *
 *  putchar, puts, printf
 *
 *  getchar, gets, scanf
 */

extern int __focos;
uint8_t *sysiomem= (uint8_t *)0x400000;


void cls()
{
    int70(SYS_CALL_CLS,0,0,0);
}


void color(int val)
{
    int70(SYS_CALL_COLOR,0,0,val);

}

int8_t putchar(int8_t ch)
{

    return int70(SYS_CALL_CHAR,0,0,ch);

}

void puts(char *s)
{

    int i;
	if(!s)return;
	 
	for(i=0;i <strlen(s);i++)
	putchar(s[i]);
   
}


void printf(const char *format,...)
{


	char* ap;
	va_start (ap,format);
	int index = 0;
	uint8_t u;	
	int d;
	char c, *s;
	char buffer[256];

	while (format[index])
	{
		switch (format[index])
		{
		case '%':
			++index;
			switch (format[index])
			{
			
     case 'C':
			case 'c':
				c = (int8_t) va_arg (ap, int32_t);
				putchar(c);
				break;
     
     case 'S':
			case 's':
				s = va_arg (ap, int8_t*);
				puts(s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int32_t);
				atoi(d, buffer);
				puts(buffer);
				break;

			case 'U':
			case 'u':
				u = va_arg (ap, uint32_t);
				atoi(u, buffer);
				puts(buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int32_t);
				i2hex(d, buffer,8);
				puts(buffer);
				break;
			
			default:
				putchar('%');
				putchar('%');
				break;
				
			}
			break;

		default:
			putchar(format[index]);
			break;
		}
		++index;
    }
}



int8_t getchar()
{
	int8_t value = 0;
	while((__focos - (*(uint8_t*)(sysiomem))));
	while (!((*(uint8_t*)(sysiomem + 0x20))&0xff));
	value = (*(uint8_t*)(sysiomem + 0x20))&0xff;
	(*(uint8_t*)(sysiomem + 0x20)) = 0x00;
      	//putchar(value);
   	return (int8_t) (value); 

	
}


void gets(int8_t *buffer){

    int32_t i=0;
    int8_t ch=0;
    int32_t size = sizeof(buffer);

	 while(TRUE)
    {
	    ch = getchar();
        if(ch == '\n')break;
		else if(ch == '\b' && i > 0){buffer[--i] = '\0';putchar(ch);}
		else if(ch != '\b'){buffer[i++] =ch;putchar(ch);}
    }

   buffer[i++] ='\0';
   putchar('\n');


}

void getsn(int8_t *buffer, int n)
{

    	int32_t i=0;
    	int8_t ch=0;
    	int32_t size = sizeof(buffer);
	while(TRUE) {
		ch = getchar();
            	if(ch == '\n')break;

		if(size-1) {
			if(ch == '\b' && i > 0) {
				buffer[--i] = '\0';
				n++;
			}
			else if(ch != '\b') {
				buffer[i++] =ch;
				n--;
			}
		} else {     

			
		}
    	}

   	buffer[i++] ='\0';
	  


}



void scanf(const char *format,...){


    

}
