#include <io.h>
#include <stdarg.h>
#include <string.h>
#include <drivers/console.h>
#define	INT_MAX	2147483647	// max value of an "int"

static void i2hex(uint32_t val, char* dest, int len);
static void itoa(int value, char* valuestring);

uint16_t *video_memoria = (uint16_t*) 0xB8000;
uint8_t color =0x0F;  //atributo do caracter
int cursor_x =0;
int cursor_y =0;


// actaulizador do cursor 
void update_cursor()
{
    	uint16_t posicao_cursor = cursor_y *80 + cursor_x;
	outportb(0x3D4,0xE);
	outportb(0x3D5,(uint8_t)(posicao_cursor >> 8) &0xFF);
	outportb(0x3D4,0xF);
	outportb(0x3D5,(uint8_t)(posicao_cursor) &0xFF);
}

void set_cursor(int x, int y)
{	
	cursor_x = x;
	cursor_y = y;
    	update_cursor();
	
}
void set_cursor_x(int x)
{
	cursor_x = x;
    	update_cursor();
	
}
void set_cursor_y(int y)
{
	
	cursor_y = y;
    	update_cursor();
}
	
void set_color(const unsigned c)
{
	color = (c & 0x0F);
}



void cls()
{

  	uint16_t *memoria = (uint16_t*)video_memoria;
  	int i=0;
  	int attr = color << 8;
  	while(i < 80*25)
  	{       
       		*memoria++ = attr | ' ';
	    	i++;
	
  	}
	set_cursor(0,0);
	
}
		
void putchar(uint8_t ch)
{
	
	uint16_t *posicao = video_memoria + ((cursor_y *80) + cursor_x);
	uint16_t atributo = color << 8;

	
	if(ch == '\b'&&cursor_x){
		cursor_x--;
		putchar(' ');
		cursor_x--;
	 }
	 
    else if(ch == '\t'){
		(cursor_x += 8);//&~(8-1);
		}
		
    else if(ch == '\n'){
		cursor_x = 0;
		cursor_y++;
		update_cursor();
	
		}
		
	else if(ch >= ' '){
	
		*posicao = ch | atributo;
    	 cursor_x++;
	}
	
	if(cursor_x >= 80){
		cursor_x =0;
		cursor_y++;
		}
		
	if(cursor_y >= 25){
	
		scroll();
		
		}
		
	update_cursor();
 }

 

void puts(const char* s)
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
	unsigned char u;	
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
				c = (char) va_arg (ap, int);
				putchar(c);
				break;
     
    			case 'S':
			case 's':
				s = va_arg (ap, char*);
				puts(s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int);
				itoa(d, buffer);
				puts(buffer);
				break;

			case 'U':
			case 'u':
				u = va_arg (ap, uint32_t);
				itoa(u, buffer);
				puts(buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int);
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
			putchar(format[index]); //
			break;
		}
		++index;
	}
}

void scroll()
{
	
	int i;
	uint16_t atributo;

	if(cursor_y >= 25)atributo = color << 8;
	for(i=0;i<25*80;i++)video_memoria[i]=video_memoria[i+80];
	for(i=25*80;i<25*80;i++)video_memoria[i]=atributo |' ';
	cursor_y =24;

}

static void i2hex(uint32_t val, char* dest, int len)
{
	char* cp;
	char x;
	uint32_t n;
	n = val;
	cp = &dest[len];
	while (cp > dest)
	{
		x = n & 0xF;
		n >>= 4;
		*--cp = x + ((x > 9) ? 'A' - 10 : '0');
	}
    dest[len]='\0';
    //dest[len+1]='\0';
}



static void itoa(int value, char* valuestring)
{
  int min_flag;
  char swap, *p;
  min_flag = 0;

  if (0 > value)
  {
    *valuestring++ = '-';
    value = -INT_MAX> value ? min_flag = INT_MAX : -value;
  }

  p = valuestring;

  do
  {
    *p++ = (char)(value % 10) + '0';
    value /= 10;
  } while (value);

  if (min_flag != 0)
  {
    ++*valuestring;
  }
  *p-- = '\0';

  while (p > valuestring)
  {
    swap = *valuestring;
    *valuestring++ = *p;
    *p-- = swap;
  }
}
