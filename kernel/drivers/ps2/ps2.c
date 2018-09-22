#include <io.h>
#include <drivers/ps2.h>
#include <irqs.h>


// Esta função é usada para a calibragem do IBF e OBF
// Se a entra é 0 = IBF, se entrada é 1 = OBF
void kbdc_wait(uint8_t type)
{
	int spin = 100000; 

	if(type == 0) {
		while(!(inb(0x64)&1)) { wait_ns(100); if(!(spin--))break; }
       	}

	else if(type == 1) {

                 while((inb(0x64)&2)) { wait_ns(100); if(!(spin--))break; }

      	}else wait_ns(200);

}



// Aqui! Faremos a configuração do PS/2 habilitando o auxiliary device (mouse)
void ps2_install(){
	
	uint8_t tmp;


    	//Desativar os dispositivos PS/2, 
    	//isto envita que os dispositivos PS/2 
    	//envie dados no momento da configuração.
	cli();


    	//Desativar a primeira porta PS/2
  	kbdc_wait(1);
	outportb(0x64,0xAD);
    
    	//Desativar a segunda porta PS/2  
	kbdc_wait(1);
	outportb(0x64,0xA7);

  
    	//Defina a leitura do byte actual de configuração do controlador PS/2
	kbdc_wait(1);    
	outportb(0x64,0x20);


    	//Activar o segundo despositivo PS/2, modificando o status de configuração do controlador PS/2.
    	//Lembrando que o bit 1 é o responsável por habilitar, desabilitar o segundo despositivo PS/2  (o mouse).
	kbdc_wait(0);
	tmp=inportb(0x60)|2; 

	// defina, a escrita  de byte de configuração do controlador PS/2
	kbdc_wait(1);
	outportb(0x64,0x60);

    	// devolvemos o byte de configuração modificado
	kbdc_wait(1);
	outportb(0x60,tmp);  
	


    	//Agora temos dois dispositivos sereais teclado e mouse (PS/2).
	

    	//Activar a primeira porta PS/2
	kbdc_wait(1);
	outportb(0x64,0xAE);  

    	// activar a segunda porta PS/2
	kbdc_wait(1);
	outportb(0x64,0xA8);

     	// espera   
	kbdc_wait(1);
	
	sti();


}
