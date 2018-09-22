#include <io.h>
#include <drivers/ps2.h>
#include <irqs.h>
#include <drivers/mouse.h>

#define SINAL_N -
#define SINAL_P +



static void mouse_refresh();

const char *id_mouse_strings[]={
    "Default",
    "Unknown",
    "Unknown",    
    "IntelliMouse"
};



// TODO: Em minha configuração do mouse, levo apeito o sinal das variáveis...
signed char buffer_mouse[3];
uint8_t     buttons;
signed int  mouse_x;
signed int  mouse_y;


// Algumas variáveis de controle

static uint32_t largura_da_tela = 1024;
static uint32_t altura_da_tela  = 768;
uint8_t speed = 1;
uint32_t mouse_position=0;


// Esta função será usada para escrever dados do mouse na porta 0x60, fora do IRQ12
void mouse_write(uint8_t write){

	kbdc_wait(1);
	outb(0x64,0xD4);
	kbdc_wait(1);
	outb(0x60,write);
    	wait_ns(200);

}


// Esta função será usada para ler dados do mouse na porta 0x60, fora do IRQ12
uint8_t mouse_read(){

    
	kbdc_wait(0);
	uint8_t val = inportb(0x60);
    	wait_ns(200);
    	return val;

}


// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
static _Bool BAT_TEST(){
    
    	uint8_t val;

    	while(TRUE){

        	val = mouse_read();

        	if(val == KBDC_OK)return 0;
        	else if(val == KBDC_ERROR) {
       			return -1; 
       		}
    
        	// Reenviar o comando. 
        	// OBS: este comando não é colocado em buffer
        	mouse_write(KBDC_RESEND);       
        

        }

}



// Instalação do mouse
void mouse_install(){


    	cli();

    	// reseta o mouse
	mouse_write(KBDC_RESET);
    	//Espera o dados descer (ACK)
    	while( mouse_read() != KBDC_ACK);


    	// Basic Assurance Test (BAT)
    	if(BAT_TEST() != 0) {

    		// Aqui! Precisaremos de fazer alguma coisa, em casos de erro
    		printf("\n Mouse error!");

    	}

    	// Use mouse default
	mouse_write(MOUSE_DEFAULT);
   	//Espera o dados descer (ACK)
    	while( mouse_read() != KBDC_ACK);


    	// habilita o mouse.
	mouse_write(ENABLE_MOUSE);
    	//Espera o dados descer (ACK)
	while( mouse_read() != KBDC_ACK);


    	// espera nossa controladora terminar
    	kbdc_wait(1);

    	// habilita o IRQ12
	irq_enable(12);
    	sti();

}

// Este é o manipulador do IRQ12
void mouse_handler(void){

static int first_time = 1;
static int count_mouse=0;


    	// DICA DO TIO FRED
    	// Ao que parce ignorar a leitura do primeiro byte
    	// coloca nossos dados na ordem status, x, y
    	if (first_time != 0) {
        	first_time = 0;
        	inb(0x60);
    	}else {

		buffer_mouse[count_mouse++]=inportb(0x60);
	
    		if(count_mouse==3) {
    			buttons = buffer_mouse[0];	
			mouse_refresh();
			count_mouse=0;
	    	}

    	}		

}

// Actualizador do ponto de acomulação do mouse
static void mouse_refresh(){

/*                                      Mouse default (0xF6)
 *
 *   _____________________________________________________________________________________________
 *  { overflow y | overflow x | signal y | signal x| always 1 | middle btn | right btn | left btn }
 *  {____________|____________|__________|_________|__________|____________|___________|__________}
 *  {                                       Delta X                                               }
 *  {_____________________________________________________________________________________________}
 *  {                                       Delta Y                                               }
 *  {_____________________________________________________________________________________________}
 *
 *
 */    

     	// Em nossa configuração, não precisamos nos preocupar com o SIGNAL, 
     	// ele é tratado nos cálculos, tendo enconta o sinal da variável (signed)
     	// O nosso mouse_y crescerá para baixo...
     	mouse_x =(buttons &0x40)? mouse_y  : mouse_x + buffer_mouse[1] / speed;
     	mouse_y =(buttons &0x80)? mouse_y  : mouse_y - buffer_mouse[2] / speed;

     	if(mouse_x < 0) mouse_x =0;
     	else if(mouse_x >largura_da_tela) mouse_x = largura_da_tela;
     	if(mouse_y < 0) mouse_y =0;
     	else if(mouse_y >altura_da_tela ) mouse_y = altura_da_tela;

     	mouse_position = mouse_x + (largura_da_tela  * mouse_y);
    
    	/* cls();
     	printf("((%i)   (%i,%x,%i)) ",mouse_position, mouse_x, buttons ,mouse_y ); */
    

}
