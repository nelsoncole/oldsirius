#ifndef __ASM_H__
#define __ASM_H__
#define SYS_CALL_CLS    32
#define SYS_CALL_CHAR   33
#define SYS_CALL_COLOR  35
#define SYS_CALL_CHARSET 36


#define int70(eax,ebx,ecx,edx)({\
unsigned int ret_val;\
__asm__ __volatile__("\
int $0x69":"=a"(ret_val):"a"((eax)),"b"(ebx),"c"(ecx),"d"(edx));\
ret_val;\
})






#endif

