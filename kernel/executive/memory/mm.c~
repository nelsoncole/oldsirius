#include <io.h>
#include <string.h>
#include <memory/page.h>
#include <memory/mm.h>
#include "mmap.h"


uint32_t kalloc_d = 0;
uint32_t kalloc_t = 0;
uint8_t busy = 1;

static int alloc_frame_spin_lock = 0;

static void init_physical_memory(uint32_t arg);
void mem_cofg(uint32_t arg)
{

	char *mboot = (char*)arg;
	init_physical_memory((*(uint32_t*)(mboot + 16)));





}


void kinit_alloc () 
{
	
	uint32_t addrtble;
   	uint32_t physical = 0xA00000;
    	uint32_t d= kalloc_d = page->end_kdirectory;
    	uint32_t t= kalloc_t = (page->used_kdirectory);

    	int i,tmp = d +1;
   
    	for(;d < tmp;d++) {
          
            for(i=0;i<1024;i++) {
		kpage_table[(t*1024) +i].p = 0;
		kpage_table[(t*1024) +i].rw= 1;
		kpage_table[(t*1024) +i].g=  1;
		kpage_table[(t*1024) +i].frames = ((physical+(i*0x1000)) >>12) &0xFFFFF;
	
            }

		kpage_directory[d].p = 1;
		kpage_directory[d].rw= 1;
		addrtble = (uint32_t)(kpage_table + (t*1024));
		kpage_directory[d].addrpt = (addrtble >>12) &0xFFFFF;

		t++;
	
          
        }

    	flush_tlb(); // Actualiza TLB

}

// Alocaremos 1 pagina em apenas (4KB)

void *kalloc(size_t size)
{
    
    	uint32_t d= kalloc_d;	
	uint32_t t= kalloc_t;

	int x = 0,i =0;	

label:
    	while(kpage_table[(t*1024) +i].p) {
    		if(i == 1024) {

			printf("kalloc error\n");
			die();
			return (uint32_t*) (-1);
		} 

		i++;

    	}


	for(x=0; x < size; x++) {
		if((kpage_table[(t*1024) +i +x].p)) { // is hole
			i += x;
			goto label;
		}

	}

	for(x=0; x < size; x++) {
		kpage_table[(t*1024) +i +x].ign = 1;
    		kpage_table[(t*1024) +i +x].p = busy;
	}

	kpage_table[(t*1024) + i + (x-1)].ign = 0;


	kpage_table[(t*1024) +i +x].p = busy;

    	//flush_tlb();
   	return (uint32_t*)((d *4096 *1024) +(i*0x1000));
}

void kfree_alloc(void *free) 
{
    	uint32_t d = ((uint32_t)free >>22) &0x3ff;
    	uint32_t t = ((uint32_t)free >>12) &0x3ff;
    	int i;

	d -=kalloc_d;
  
    	for(i=0;i<1024;i++) {
    		kpage_table[((kalloc_t +d) *1024 ) + t +i].p &= ~busy;
		if(!(kpage_table[((kalloc_t +d) *1024) + t +i].ign))break;

		kpage_table[((kalloc_t +d) *1024) + t +i].ign = 0;
    	}
}

/*
    31----------------­----22--------------­-------------12------------------­---------- 00 
    |--------directory-­-­---|--------page table----------|-----­­---offset----------­-­-----|
    
*/

void *mem_map(void * physaddr, uint8_t flg, size_t size)
{
	uint32_t virtualaddr = mmap_device;
	uint32_t addrtble;
   	uint32_t physical = (uint32_t)physaddr;
   
    	int d = (int)virtualaddr >> 22 & 0x3FF;
    	int t = (int)virtualaddr >> 12 & 0x3FF;
    	//int o = (int)virtualaddr  & 0xFFF; // unused


	uint32_t  i = 0,x = (d -768) +1;

    	uint32_t a = (size/1024);
	uint32_t z,b = (size%1024);

    	do {

		if(!a) z = t+b;
		else z = 1024;
          
            	for(;t < z ;t++) {
			kpage_table[(x*1024) + t].p = flg &1;
			kpage_table[(x*1024) + t].rw= (flg >>1)&1;
			kpage_table[(x*1024) + t].pcd= (flg >>4)&1;
			kpage_table[(x*1024) + t].g  = (flg >>8)&1;		
			kpage_table[(x*1024) + t].frames = ((physical+(0x1000*i)) >>12) &0xFFFFF;
			i++;
	
        	}
          
 
	kpage_directory[d].p = flg &1;
	kpage_directory[d].rw= (flg >>1)&1;
	kpage_directory[d].pcd= (flg >>4)&1;
	addrtble = (uint32_t)(kpage_table + (x*1024));
	kpage_directory[d].addrpt = (addrtble >>12) &0xFFFFF;


	d++;
	x++;
	t =0;


	}while(a--);



             
    	flush_tlb(); // Actualiza TLB

    	mmap_device += (0x1000*size);

    	return (uint32_t*) (virtualaddr);
 
    
}

struct {
    	uint32_t sizel;
	uint32_t sizeh;
	uint32_t bmp;
	uint32_t pointer;
    	uint32_t total_num_of_cluster;
	uint32_t reserved;
	uint32_t holes;
    	uint32_t busy;
    	uint32_t free;
}ram;

extern uint32_t ram_pointer;
static uint32_t ram_cluster(uint32_t speed);
static void init_physical_memory(uint32_t arg)
{

    	char *mboot = (char*)arg;
    	int i;
    	ram.sizel = (*(uint32_t*)(mboot + 0))/1024;
	ram.sizeh = (*(uint32_t*)(mboot + 4))/1024;
	ram.bmp  = (uint32_t)(mboot + 8);

	//uint32_t *bmp = (uint32_t *)ram.bmp; // unused

	ram.reserved = (32*1024);      // 32MB, esta é a área do kernel + a conevencional.


	ram.holes = (0); //TODO devemos pegar informações da memória via  INT 15h, EAX=0xE820 ou CMOS

    	ram.busy = (ram.sizel - ram.reserved - ram.holes);
    	ram.free = (ram.sizel - ram.busy);
	ram.pointer =(uint32_t) &ram_pointer;


	ram.total_num_of_cluster = (ram.sizel/4);

    	uint32_t *tmp = (uint32_t*)ram.pointer;

	memset(tmp,0,(ram.total_num_of_cluster*4)); // assume que a área da ram fat, seja zero

    	for(i = 0;i < ((ram.reserved) /4); i++){

    		tmp[i] =0xFFFFFFF8;
    
    	}

    	for(i=0; i < ram.holes; i++){
    		//TODO, Nelson aqui vamos marcar como ocupado os buracos de memoria fisica    
    
    	}

	printf("RAM %d.%d%dGB (%dMB)\n",(ram.sizel/1024)/1024,(ram.sizel/1024)%1024/100,(ram.sizel/1024)%1024/10,(ram.sizel/1024));

		

}

uint32_t *alloc_frame(page_table_t *pte,uint8_t flg, size_t count)
{

	while(alloc_frame_spin_lock)  nop();

	alloc_frame_spin_lock++;


	if(!count)return (uint32_t*) -1;
	uint32_t *tmp = (uint32_t*)ram.pointer;
	uint32_t cluster = 0;
    	uint32_t fist_cluster = ram_cluster(0);
    	uint32_t virtual = (fist_cluster *0x1000);

    	int i =0;

    	while(count--) {    

    		if(fist_cluster == -1)return (uint32_t*) -1;       
    		else if(!count) {
        		tmp[fist_cluster] = 0xFFFFFFFF;

			pte[i].p = (flg &1);
			pte[i].rw= (flg >>1 &1);
			pte[i].us= (flg >>2 &1);
			pte[i].frames = ((fist_cluster* 0x1000) >> 12) &0xFFFFF;
			i++;
        
    		}else {
        		cluster = ram_cluster(fist_cluster);
        		tmp[fist_cluster] = cluster;

			pte[i].p = (flg &1);
			pte[i].rw= (flg >>1 &1);
			pte[i].us= (flg >>2 &1);
			pte[i].frames = ((fist_cluster* 0x1000) >> 12) &0xFFFFF;
			i++;
        		fist_cluster = cluster;
        
        	}

    	}  
	
	alloc_frame_spin_lock = 0;
    	return (uint32_t*) (virtual);

    
}


void free_frame(void *free)
{
	uint32_t p = (uint32_t)free;

    	uint32_t fist_cluster = (p/0x1000);
    	uint32_t *tmp = (uint32_t*)ram.pointer;
    	uint32_t index;
    
    	index = tmp[fist_cluster];    

    	while(true){
    	if(index != 0xFFFFFFF8)tmp[fist_cluster] = 0x0;
    	if((index == 0xFFFFFFFF) || (index == 0xFFFFFFF8))break;
    	fist_cluster = index;
    	index = tmp[index];
    
    } 

    
}

static uint32_t ram_cluster(uint32_t speed)
{

    uint32_t *tmp = (uint32_t*)ram.pointer;

    uint8_t c = 1;
    uint32_t cluster = speed; // Aqui + start
    while(tmp[++cluster]){
    if(cluster >= ram.total_num_of_cluster){
        if(!c--){
            cluster = -1;
            break;
          }
            else cluster = 0;
        }

    }

    return cluster;

}

void ram_update(){





}

