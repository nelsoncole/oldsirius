#ifndef __gui_h__
#define __gui_h__
#include <typedef.h>

typedef struct display
{
	uint16_t pitch;
	uint8_t  bpp;
	uint8_t  planes;
    	uint16_t x_resolution; //width;
    	uint16_t y_resolution; //hight;
    	uint8_t  x_char_size;
    	uint8_t  y_char_size;
	volatile uint8_t *framebuffer;
 	uint16_t byte_scan_line;
    	uint32_t max_pixel_clock;


}__attribute__ ((packed)) display_t;

int pixel_offset(int x, int y);
void put_pixel(int x, int y, int color);

void drawchar(uint8_t c, int x, int y, int fgcolor, int bgcolor);
void drawchar_transparent(uint8_t c, int x, int y, int fgcolor);
void BoxChar(int c, int color);

int __putchar_box(int c);

int vbe_init(uint32_t arg);
void gui_init(uint32_t arg);



#endif



