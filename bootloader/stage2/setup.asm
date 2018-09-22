;=========================================================================
; "start.asm"
;
; Nelson Sapalo da Silva Cole  (nelsoncole72@gmail.com  +244-948-833-132)
; Lubango, 2017-2018
;=========================================================================



bits 16
section .text

global _start
_start:
	jmp 0:start

	extern puts16
	extern gatea20
	extern gdtr
	extern bootdevice_num
	extern GetDeviceParameters
	extern init_vbe16

start:
	cli	
    	mov ax,cs
	mov ds,ax
	mov es,ax
    	mov ax,0x9000
	mov ss,ax
	mov sp,0
	sti


; Salvar o valor de disco 
	mov BYTE [bootdevice_num],dl
; BIOSes Get Device Parameters

	call GetDeviceParameters

; VESA VBE
	call init_vbe16

; Habilita Gate A20
	call gatea20

; Testa a Gate A20 

; Mascara interrupções e desabilita NMI
	
	cli 	    ;

	in al,0x70
	or al,0x80
	out 0x70,al

; Instala a GDT
    	mov eax,gdtr
	db 0x66		; Execute GDT 32-Bits
	lgdt [eax]

; Habilita o PE 
	mov eax,cr0
	or eax,1
	mov cr0,eax

; Pula para modo protegido
	jmp dword 0x8:ModoProtegido



; Retornos de Erro
booterro:
	call puts16
	pop si

; Espera uma interrupção do teclado
	xor ax,ax
	int 0x16
	int 0x18



bits 32
extern main
ModoProtegido:
    	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,0x90000	; Nossa pilha

	
	call main


	
	

; Põe o processador para dormir
L1:	cli
	hlt
	jmp L1
