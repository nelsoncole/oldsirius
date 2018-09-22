#include <io.h>
#include <irqs.h>
#include <drivers/ps2.h>
#include <drivers/keyboard.h>

extern int os_ready;
uint32_t *input = (uint32_t *)0x400020;

// Algumas macros define para o keyboard
#define ESC 0x1B
#define	TAB '\t'
#define ENTER '\n'
#define	SPACE ' '
#define BACKSPACE '\b'

#define	KF1 0x80
#define KF2 0x81
#define KF3 0x82
#define KF4 0x83
#define KF5 0x84
#define KF6 0x85
#define KF7 0x86
#define KF8 0x87
#define KF9 0x88
#define KF10 0x89
#define KF11 0x8A
#define KF12 0x8B
#define NUMELOCK 0
#define SCROLLOCK 0

#define DEL 0x91
#define HOME 0x92
#define END 0x93
#define PGUP 0x94
#define PGDN 0x95
#define UP 0x96
#define LEFT 0x97
#define DOWN 0x98
#define RIGHT 0x99

#define ALT	0
#define CTRL	0
#define LSHIFT	0
#define RSHIFT	0
#define PRTSCR	0

#define CAPSLOCK 0


char _charset =0;



// Nossas estáticas
static void LEDs(int n);




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
uint8_t keyboard_read(){
 
    
	kbdc_wait(0);
	uint8_t val = inportb(0x60);
    	wait_ns(200);
    	return val;

}


// Esta função será usada para escrever dados do teclado na porta 0x60, fora do IRQ1
void keyboard_write(uint8_t write){

	kbdc_wait(1);
	outb(0x60,write);
    	wait_ns(200);

}


// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
static _Bool BAT_TEST(){
    
    	uint8_t val;

    	while(TRUE) {

        val = keyboard_read();

        if(val == KBDC_OK)return 0;
        else if(val == KBDC_ERROR) {
        
            return -1; 
        }
    
        // Reenviar o comando. 
        // OBS: este comando não é colocado em buffer
        keyboard_write(KBDC_RESEND);       
        

        }

}


void keyboard_install(){

    	cli();	
    	//Reseta o teclado
	kbdc_wait(1);
	outb(0x60,KBDC_RESET);
	wait_ns(200);
	
    	// Espera os dados descer, ACK
    	while(keyboard_read() != KBDC_ACK);
    
    	// Basic Assurance Test (BAT)
    	if(BAT_TEST() != 0) {


    	// Nelson aqui precisaremos de criar uma rotina de tratamento de erro do teclado

        printf("\nkeyboard error!");

    	}  

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
void keyboard_handler(void){


    	// Algumas variáveis estática, para o controle 
    	static int shift = 0;
    	static int caps_lock = 0;

    	uint8_t scancode = inportb(0x60);

    	if(scancode & 0x80) {

        	if(scancode == 0xAA || scancode == 0xB6)
        	shift = 0;

    	} else {
            	if(scancode == 0x2A || scancode == 0x36) {
            		shift = 1;
            		return;
            	}



    	}

    	if(scancode & 0x80) {
    
        // tecla solta

    	} else {


         	if(shift || caps_lock) {
                	_charset = ascii_maiusculas[scancode];

        	} else {

                	_charset = ascii_minusculas[scancode];

            	}


		if(!(os_ready)) *input = _charset;

        }	

 
}


// Esta função estática lida com os LEDs
// FIXME: Falca configurar melhor esta rotina
static void LEDs(int type)
{

    	static uint8_t led_status = 0;    
  
    	keyboard_write(KBDC_LED);
    	// espera os dados descer (ACK)
    	while(keyboard_read() != KBDC_ACK);
    	keyboard_write(led_status);
    	// espera os dados descer (ACK)
    	while(keyboard_read() != KBDC_ACK);

    	// Espera terminar
    	kbdc_wait(1);
    
   
}



// esta função está sendo implementada para o getchar e gets
uint8_t charset()
{
    	char value = _charset;
    	_charset =0;
    	return value;
}

