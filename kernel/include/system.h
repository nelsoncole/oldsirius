#ifndef __system_h__
#define __system_h__
#include <typedef.h>

#define APP_MEMORY_VIRTUAL_START 0x2000000
#define APP_MEMORY_VIRTUAL_END (APP_MEMORY_VIRTUAL_START + 0x400000)

void reboot();
int execve(char *filename,int argc,char *argv[]);




#endif
