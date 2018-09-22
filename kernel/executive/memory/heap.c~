#include <io.h>
#include <string.h>
#include <memory/heap.h>

extern uint32_t start_heap; 
extern uint32_t end_heap;

// algumas macros define para o heap do kernel
// Estou tentando criar um kheap dentro dos 16MiB
#define KHEAP_START (&start_heap) // TODO Os primeiros 4MB apos ao end_kernel, fica para a área da tabela do alocador de frames
#define KHEAP_END   (&end_heap) 
#define KHEAP_SIZE  ((KHEAP_END - KHEAP_START))


/*                  Possível estrutura de nosso heap...
 *   _______________________________________________________________________________
 *  { (bitmap) Region |                     (data) Region                           }
 *  {_________________|_____________________________________________________________}
 *
 *
 *
 */

volatile struct{

    uint32_t start;     // inicio do bloco
    uint32_t end;       // final do bloco
    uint32_t size;      // tamanho do bloco

    uint32_t footer;    // O rodapé é para saber onde acabou o bitmap, para iniciar outro...
    uint32_t free;      // O total de memoria livre

    uint32_t f_start;       // variávei para a fragmentação
    uint32_t f_size;        // variávei para a fragmentação

    uint8_t *ptr;           // ponteiro para o início do campo de bitmap
    uint32_t start_phy;     // Inicio do espaço de dados
    uint32_t size_bitmap;   // tamanho do campo de bitmaps

}heap;


// Nossa ideia cada bloco alocado é de tamanho 4bytes
// A cada bloco é controlado por 2-bit 
// first bit se 0 livre se 1 ocupado
// second bit se 0 continua se 1 fim.
// Assim sendo: 
//              01 início
//              11 fim.
void init_heap(){

    heap.start     = (uint32_t)&start_heap; //KHEAP_START;
    heap.end       = (uint32_t)&end_heap; //KHEAP_END;
    heap.size      = (heap.end - heap.start); //KHEAP_SIZE;
    heap.footer    = 0;
    heap.free      = (heap.end - heap.start);
    heap.f_start   = 0;
    heap.f_size    = 0;
    heap.size_bitmap = ((heap.size/32)*2);
    heap.ptr     = (uint8_t*)heap.start;
    heap.start_phy =  heap.start + heap.size_bitmap; 
    memset((uint32_t*)heap.start,0,heap.size_bitmap);


   printf("Heap Install (Start: %X End: %X Size: %dMB (%d bytes))\n",heap.start,heap.end,(heap.size/1024)/1024,heap.size);

}


uint32_t malloc(size_t size)
{

    size_t i = size;
    uint32_t ret; 
    uint8_t *bitmap;
    size_t count;
    
    if(heap.free >= size){
   
    if((i % 4) !=0){ 
        i /=4;    
        i++;
    }else i/=4;

    count = ((heap.f_size != 0 && heap.f_size >= size)? heap.f_start : heap.footer);
    


    while(i--){
        bitmap = (uint8_t*)heap.ptr + count;
        *bitmap |= 1;
        count++;
    }
        *bitmap |= 3;
    
    ret = heap.start_phy + (4*((heap.f_size != 0 && heap.f_size >= size)? heap.f_start : heap.footer));

    if(heap.f_size != 0 && heap.f_size >= size){
        heap.f_start = count;
        heap.f_size -= size; 
    
    }else heap.footer = count;
    
    heap.free -= size;

	return (uint32_t)(ret);

    }else return (uint32_t)(-1); // Em caso de sem espaço livre precisamos de um tratador
	

}


void free(void *buffer)
{
    
    size_t count =0;
    uint32_t a = (uint32_t)buffer - heap.start;
    a = (a - heap.size_bitmap)/4;
    
    int tmp_a = a; 
   
        
    uint8_t *bitmap;
    do{

        bitmap = (uint8_t*)heap.ptr + a;
        if(*bitmap == 3){
        *bitmap &= 0xFC;
        count++;
        break;
    }
    *bitmap &= 0xFC;
    a++;
    count++;   
    
    }while(TRUE);

    heap.free += (count*4);


    heap.f_start = tmp_a;
    heap.f_size = (count*4);
 	
     	
}
