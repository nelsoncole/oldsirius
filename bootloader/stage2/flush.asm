;==========================================================================================
; "flush.asm"
; 
; Nelson Sapalo da Silva Cole  (nelsoncole72@gmail.com  +244-948-833-132)
; Lubango, 2017-2018
;========================================================================================
BITS 32
section .text
extern bootdevice_num
GLOBAL gdt_flush

	gdt_flush:
    	push ebp
	mov ebp,esp
    	mov eax, [esp+8]
    	lgdt [eax]
    	jmp 0x08:.flush
.flush:
    	mov ax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
    	mov esp,ebp
	pop ebp
    	ret

GLOBAL lidt_install
EXTERN idt
lidt_install:

    	push ebp
	mov ebp,esp

    	mov eax,idt_ptr
    	lidt [eax]
    	mov esp,ebp
	pop ebp
    	ret



IDT_LIMIT equ $ - 48
idt_ptr:
	dw IDT_LIMIT - 1
	dd idt

global execkernelx86
execkernelx86:
    	mov eax,[esp+4]
	mov ebx,[esp+8]

	cli
	push ebx
	push ss
	push esp
	pushf
	push cs
    	push eax
	iretd

section .data 
