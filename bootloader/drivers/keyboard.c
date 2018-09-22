#include <io.h>
#include <irqs.h>
#include <drivers/keyboard.h>
// Algumas macros define KBDC //KEYBOARD

#define KBDC_RESET 0xFF
#define KBDC_RESEND 0xFE
#define KBDC_ERROR 0xFC
#define KBDC_ACK 0xFA
#define KBDC_ID 0xF2
#define KBDC_OK 0xAA
#define KBDC_LED 0xED



// Algumas macros define para o keyboard
#define ESC 0x1B
#define	TAB '\t'
#define ENTER '\n'
#define	SPACE ' '
#define BACKSPACE '\b'

static char _charset =0;


static void kbdc_wait(uint8_t type){
        
    
	int spin = 100000; 

	if(type == 0) {
		while(!(inb(0x64)&1)) { wait_ns(100); if(!(spin--))break; }
       	}

	else if(type == 1) {

                 while((inb(0x64)&2)) { wait_ns(100); if(!(spin--))break; }

      	}else wait_ns(200);

}





// Este é o nosso mapa de caracters minúsculas
const char ascii_minusculas[] = {
	NULL,ESC,'1','2','3','4','5','6','7','8','9','0','-','=',BACKSPACE,
	TAB,'q','w','e','r','t','y','u','i','o','p','[',']',ENTER,
	0,'a','s','d','f','g','h','j','k','l',';','\'','`',
	0,'\\','z','x','c','v','b','n','m',',','.','/',0,0,0,
	SPACE
};


// Este é o nosso mapa de caracters maiúsculas
const char ascii_maiusculas[] = {
	NULL,ESC,'!','@','#','$','%','^','&','*','(',')','_','+',BACKSPACE,
	TAB,'Q','W','E','R','T','Y','U','I','O','P','{','}',ENTER,
	0,'A','S','D','F','G','H','J','K','L',':','"','~',
	0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,0,
	SPACE
};




// Esta função será usada para ler dados do teclado na porta 0x60, fora do IRQ1
static uint8_t keyboard_read(){
 
    
	kbdc_wait(0);
	uint8_t val = inb(0x60);
    	return val;

}


// Esta função será usada para escrever dados do teclado na porta 0x60, fora do IRQ1
static void keyboard_write(uint8_t write){

	kbdc_wait(1);
	outb(0x60,write);
    	wait_ns(100);

}


void keyboard_install(){

    
    	cli();	
    	//Reseta o teclado
	kbdc_wait(1);
	outb(0x60,KBDC_RESET);
    	// Espera os dados descer, ACK
    	while(keyboard_read() != KBDC_ACK);

    	// espera nossa controladora termina
	kbdc_wait(1);


    	// Habilita IRQ1
	irq_enable(1);
    	sti();

        

}


// Este é o nosso manipulador do mouse no vetor IDT
// TODO: 
// O Bit 7 (0x80) na scancode lido, nos informa se a tecla foi precionada ou solta  		
// Exemplo: O shift left quando precionado gera o scancode 0x2A quando solto 0xAA (0x2A + 0x80 = 0xAA)
void keyboard_handler(void)
{
    	// Algumas variáveis estática, para o controle 
    	static int shift = 0;
    	static int caps_lock = 0;

    	uint8_t scancode = inb(0x60);

    	if(scancode & 0x80){

        if(scancode == 0xAA || scancode == 0xB6)
        	shift = 0;

    	} else {
            	if(scancode == 0x2A || scancode == 0x36){
            	shift = 1;
            	return;
        }



    	}

    	if(scancode & 0x80){
    
        // tecla solta

    	}else {


         if(shift || caps_lock){
                 _charset = ascii_maiusculas[scancode];
                 

        }else{

                _charset = ascii_minusculas[scancode];
		


                }

        }	

 
}




// esta função está sendo implementada para o getchar e gets
uint8_t charset(){
    char value = _charset;
    _charset =0;
    return value;
}

