bits 32
section .text

	extern main

	global _start
_start:
	jmp start
Align 4
mboot:
    	MULTIBOOT_HEADER_MAGIC	equ 0x454C4F43
    	MULTIBOOT_HEADER_FLAGS	equ 0x80000001
	extern _end
    	dd MULTIBOOT_HEADER_MAGIC
    	dd MULTIBOOT_HEADER_FLAGS    
    	dd mboot
	dd 0
	dd _start
    	dd _end

start:
	mov eax,0x10
	mov ds, ax
	mov ss, ax
	mov esp,stack

	push ebx
	call main
	add esp,4

L1:	cli
	hlt
	jmp L1

	global hello
hello:
	push ebp
	mov ebp,esp
	mov esi,[esp+8]
	call print
    	mov esp,ebp
	pop ebp
	ret
msg: db "Hello Kernel!",0

	
print:
	xor edi,edi
	pushad
	mov edi,[PutStr_Ptr]
.next:
	lodsb
	cmp al,0
	jz .end
	mov ah,0x2
	cld
	stosw
	jmp .next
.end:
	popad
	ret

PutStr_Ptr dd 0x0B8000


section .data
section .bss
	global ram_pointer
	resb 0x400000
ram_pointer:
	global stack
	resb 0x8000
stack:
	global start_heap
	global end_heap
start_heap:
	resb 0x400000
end_heap:  

