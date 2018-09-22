#include <gui/gui.h>

extern display_t *display;
int pixel_offset(int x, int y)
{
    	return (y * display->pitch + (x*(display->bpp/display->x_char_size)));
}
 
void put_pixel(int x, int y, int color)
{
    
	switch (display->bpp) {
	case 16:
		(*(uint16_t*)(display->framebuffer + pixel_offset(x,y) )) = color;
		break;
	case 24:
		(*(uint16_t*)(display->framebuffer + pixel_offset(x,y) )) = color;
		(*(uint8_t*)(display->framebuffer + pixel_offset(x,y) +2)) =color >>16 &0xff;
		break;
	case 32:
    		(*(uint32_t*)(display->framebuffer + pixel_offset(x,y) )) = color;
		break;
	}
}


