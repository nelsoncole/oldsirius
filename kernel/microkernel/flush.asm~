bits 32
section .text
	extern stack
	global gdt_flush
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
	mov ss, ax
	mov esp,stack
    	mov esp,ebp
	pop ebp
    	ret

	global idt_flush
idt_flush:
    	push ebp
	mov ebp,esp
    	mov eax, [esp+8]
    	lidt [eax]
    	mov esp,ebp
	pop ebp
    	ret
