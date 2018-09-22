bits 32
section .text
	global int69
int69:   
	push ds
	push es
    	push ss
	push fs
	push gs

    	push ebx
    	push ecx
    	push edx
    	push edi
    	push esi
    	push ebp
    	push esp

    	push eax

	mov eax,0x10 ; + 4    ; DS no LDT
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

    	extern syscall_handler
	call syscall_handler
    	add esp,4
    	pop esp
    	pop ebp
    	pop esi
    	pop edi
    	pop edx
    	pop ecx
    	pop ebx

	pop gs
	pop fs
    	pop ss
	pop es
	pop ds
    
	iret
