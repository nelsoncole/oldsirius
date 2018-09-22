#include <io.h>
#include <gui/gui.h>

display_t *display = 0;
static void gui_server();

void gui_init(uint32_t arg)
{
	display = (display_t*)kalloc(1);

	vbe_init(arg);


	create_task(&gui_server,(page_directory_t*)page->kpdbr,(uint32_t)kalloc(1),0,0);

	
	

}



static void gui_server()
{

	int x,y;

	for(y =0;y < 768;y++) {
	for(x =0;x < 1024;x++) {

		//put_pixel(x,y,0x257be4);


		//(*(uint32_t*)(display->framebuffer + (x*(display->bpp/display->x_char_size) )+(y*display->pitch))) = -1;
		

	}
	}


	
	char buf[] ="\n\n\t\t\t\tSirius Operating System Version 1.0";

	int i;
	//for(i=0;i<sizeof(buf);i++)BoxChar(buf[i],0);


	die();
}
