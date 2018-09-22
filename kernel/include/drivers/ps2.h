#ifndef __ps2_h__
#define __ps2_h__
#include <typedef.h>


// Algumas macros define KBDC

#define KBDC_RESET 0xFF
#define KBDC_RESEND 0xFE
#define KBDC_ERROR 0xFC
#define KBDC_ACK 0xFA
#define KBDC_ID 0xF2
#define KBDC_OK 0xAA



//KEYBOARD
#define KBDC_LED 0xED



//MOUSE
#define MOUSE_DEFAULT 0xF6
#define ENABLE_MOUSE 0xF4




void ps2_install();

void kbdc_wait(uint8_t type);


#endif
