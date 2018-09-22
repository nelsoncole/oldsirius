#include <io.h>
#include <idt.h>
#include <drivers/pit.h>
#include <drivers/keyboard.h>
#include <memory/page.h>
#include <memory.h>
#include <string.h>
#include <drivers/pit.h>
#define MULTIBOOT_HEADER_MAGIC 0x454C4F43

uint32_t *info =(uint32_t*) 0x100000;


typedef struct menu {
	char title[32];
	char kernel[32];
	char root;
	char partition;
}menu_t;

menu_t menu[5];

char *script[] = {
"<load>",
"</end>",
"title:=",
"kernel:=",
"root:=",
"partition:=",
0
};

extern uint8_t bootdevice_num;
extern void execkernelx86(uint32_t,void *);
static int menucpy(char *dest,const char *src);
void main(int *arg)
{

	int spin = 100000;
	int  i =0;
	char *boot_lst ="d0:/Boot/Sirius/boot.lst";
	char *filename ="d0:/Kernel/kernel.bin";
	char *addr =(char*)KERNEL_PHYSICAL_MEM;
	char *buffer=(char*)malloc(0x1000);
	
	int offset =0;

	uint32_t magic = 0;
	uint32_t eflag = 0;
	uint32_t mboot = 0;
	uint32_t code = 0;
	uint32_t start = 0;
	uint32_t end = 0;

    	idt_install();
    	timer_install(1);
    	keyboard_install();
	ata_initialize();
	part_initialize();

	
	

	cls();


	printf("           Sirius Boot Manager Version 1.0 (C) Copyright 2017-2018\n");
	printf("_______________________________________________________________________________\n\n\n");

	if((vfs_read(buffer,boot_lst)) !=0){
	printf("Loading %s error\n",boot_lst);
	
	die();
	}else {
		
		for(i =0; ;i++) {

			if(!(strncmp(buffer + i,'#',1))) {
				//continue;
			}
			if(!(strncmp(buffer + i,script[0],6))) {

				for(i +=6;;i++) {

					spin--;
					if(!(strncmp(buffer + i,script[2],7)))
						menucpy(menu[0].title,buffer + i +7);						

					else if(!(strncmp(buffer + i,script[3],8))) {
						menucpy(menu[0].kernel,buffer + i +8);

						goto end; 
					}
					
					else {
 						if(spin == 0)goto end;
						continue;

					}


				}



			}
			

		}
 
end:
		printf("[x] %s \n",menu[0].title);

		//delay(90000);
	}



	if((vfs_read(addr,filename)) !=0){
		printf("loading %s error\n",filename);
		die();
	
	}else {
		
		for(i=0;i < (0x2000/4);i++) {

			magic = (*(uint32_t*)(addr+offset));
			if(magic == MULTIBOOT_HEADER_MAGIC)break;
			offset += 4;
		
		}



		magic = (*(uint32_t*)(addr + offset));
		eflag = (*(uint32_t*)(addr + offset+4));
		mboot = (*(uint32_t*)(addr + offset+8));
		code  = (*(uint32_t*)(addr + offset +12));
		start = (*(uint32_t*)(addr + offset +16));
		end   = (*(uint32_t*)(addr + offset +20));

	
		printf("offset %X magic: %X\n",offset,magic);
		printf("offset %X eflag: %X\n",4+offset,eflag);
		printf("offset %X mboot: %X\n",8+offset,mboot);
		printf("offset %X code : %X\n",12+offset,code);
		printf("offset %X start: %X\n",16+offset,start);
		printf("offset %X end  : %X\n",20+offset,end);

			//die();

		if(magic != MULTIBOOT_HEADER_MAGIC) {

			printf("Sirius: invalid kernel header\n");
			die();
		}
		

	}




	page_install(start/*startkernel*/,end/*enkernel*/);

	mboot_info(info);
	cls();
	execkernelx86(start/*startkernel*/,0x100000/* ebx*/);
	

}

static int menucpy(char *dest,const char *src)
{

    int i;
    for (i=0;src[i] != ';';i++) {
    	memcpy(dest+i,src +i,1);

	if(i>=32)break;	

    }
    dest[i] = 0; 
    return (i);
}
