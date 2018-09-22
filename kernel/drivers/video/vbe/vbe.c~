#include <io.h>
#include <gui/gui.h>
#include <string.h>
#include "vbe.h"

extern display_t *display;

vbe_infos_t *vbe_infos =0;
vbe_info_mode_t *vbe_info_mode =0;

int VRAMSize = 0;
int vbe_init(uint32_t arg)
{

	puts("Sirius VBE Initialize\n");

	char *mboot = (char*)(arg + 0x2000); // VBE pointer
	vbe_infos = (vbe_infos_t*)(arg + 0x2000);
	vbe_info_mode = (vbe_info_mode_t*)(arg + 0x2200);	

	VRAMSize = vbe_infos->memory_size*(64);

	if(VRAMSize > (1024*64))VRAMSize = (1024*64);

	// Mapear o LFB
	display->framebuffer = (uint8_t*)mem_map((void*)(vbe_info_mode->framebuffer),0x13,VRAMSize/4);

	display->bpp = (uint8_t) vbe_info_mode->bpp;
	display->pitch = (uint16_t)vbe_info_mode->pitch;
	display->planes =(uint8_t)vbe_info_mode->planes;
	display->x_resolution = (uint16_t)vbe_info_mode->x_resolution;
	display->y_resolution = (uint16_t)vbe_info_mode->y_resolution;
	display->x_char_size  = (uint8_t)vbe_info_mode->x_char_size;
	display->y_char_size  = (uint8_t)vbe_info_mode->y_char_size;
	display->byte_scan_line = (uint32_t)vbe_info_mode->byte_scan_line;
	display->max_pixel_clock = (uint16_t)vbe_info_mode->max_pixel_clock;
	printf("Physical LFB -> %X Virtual LFB -> %X VRAM Size %d Mb (%d kB)\nbpp -> %d\n",vbe_info_mode->framebuffer,\
	display->framebuffer,VRAMSize/1024,VRAMSize,display->bpp);
	printf("Number Of Planes -> %d\nPitch -> %d\nXResolution %d\nYResolution %d\n",display->planes,display->pitch,\
	display->x_resolution,display->y_resolution);

	printf("XChar Size -> %d\nYChar Size -> %d\nByte Scan Line -> %d\nMax Pixel Clock -> %d",display->x_char_size,\
	display->y_char_size,display->byte_scan_line,display->max_pixel_clock);


	return 0;
}
