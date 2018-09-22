[bits 32]
section .text
	global _start,__focos
_start:

	jmp start
	times 8 -($-$$) db 0
Align 4
_app:
    	APP_HEADER_MAGIC	equ 0x454C4F43
    	APP_HEADER_FLAGS	equ 0x00000000
	extern main,_end
    	dd APP_HEADER_MAGIC
    	dd APP_HEADER_FLAGS    
	dd _start
    	dd _end
	times 33 -($-$$) db 0
__focos: dd 0
start:
	mov dword[__focos],esi

	call main

	jmp $
