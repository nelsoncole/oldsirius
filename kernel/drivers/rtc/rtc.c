#include <io.h>
#include <irqs.h>
#include <drivers/rtc.h>


uint8_t status_c;

void disable_NMI(){

    outportb(0x70,inb(0x70)|0x80);

}

void enable_NMI(){

    outportb(0x70,inb(0x70) & 0x7F);
}
 


void rtc_install(){

    uint8_t status;

    cli();
    // registro status B e desabilita NMI
    outportb(0x70,0x8B);
    // ler actual valor de status B  
    status = inb(0x71);

    // registro status B 
    outportb(0x70,0x8B);

    //bit 6: Hablita interrupções periódica IRQ8,
    //bit 5 : hablita interrupção de alarme ,
    //bit 2 : caledário em formato binário,
    //bit 1 : formato 24h
    outportb(0x71, status | 0x66); 
    enable_NMI();

    // Habilita IRQ8
    irq_enable(8);
    sti();
	
}



// código c para o manipulador IRQ8 offset no IDT vetor 40
void rtc_handler(void){

 outportb(0x70,0x0C);
 // é importante ler do registro status C 
 status_c = inportb(0x71);

}





uint8_t read_time(int type){
	/*
	|------|-----------|
	| type | descricao |
	|------------------|
	| 0    | segunddos |
	| 2    | minutos   |
	| 4    | hora      |
	| 6    | dia       |
	| 7    | dia de mes|
	| 8    | mes       |
	| 9    | ano       |
	|------|-----------|
	
	
	*/
	disable_NMI();
	outb(0x70,type);
	uint8_t val =inb(0x71);
	enable_NMI();
	return val;
	}

