bits 32
section .text

	extern timer_handler
	extern task_switch
	extern task_poiter
	extern switching

save_edi    dd 0
save_eip    dd 0
save_cs     dd 0
save_eflag  dd 0
save_esp    dd 0
save_ss     dd 0

	global irq00
irq00:
	cmp byte[switching],0
	jnz _no	

    	pop dword [save_eip]
    	pop dword [save_cs]
    	pop dword [save_eflag]
    	pop dword [save_esp]
    	pop dword [save_ss]

    	mov dword [save_edi],edi
    	push edi

    	lea edi,[task_poiter]

    	mov dword[edi + 0],eax
    	mov dword[edi + 4],ebx
    	mov dword[edi + 8],ecx
    	mov dword[edi + 12],edx
       
    	mov dword[edi + 16],edi
    	mov dword[edi + 20],esi
    	mov dword[edi + 24],ebp
    	mov eax,dword[save_esp]
    	mov dword[edi + 28],eax

    	mov eax,dword[save_cs]
    	mov dword[edi + 32],eax
    	mov word[edi + 36],ds
    	mov word[edi + 40],es
    	mov word[edi + 44],fs
    	mov word[edi + 48],gs
    	mov eax,dword[save_ss]
    	mov dword[edi + 52],eax


    	mov eax,dword[save_eflag]
    	mov dword[edi + 56],eax
    	mov eax,dword[save_eip]
    	mov dword[edi + 60],eax
    

    	mov eax,cr3
    	mov dword[edi + 64],eax

    	call task_switch

    	mov eax,dword[edi + 16]
    	mov [save_edi],eax
    	mov eax,dword[edi + 0]
    	mov ebx,dword[edi + 4]
    	mov ecx,dword[edi + 8]
    	mov edx,dword[edi + 12]
    
    	mov esi,dword[edi + 20]
    	mov ebp,dword[edi + 24]


    	mov ds,word [edi + 36]
    	mov es,word [edi + 40]
    	mov fs,word [edi + 44]
    	mov gs,word [edi + 48]
    	push eax
    	mov eax,dword [edi + 52] ;ss
    	mov dword [save_ss],eax

    	mov eax,dword [edi + 28]  ;esp
    	mov dword [save_esp],eax

    	mov eax,dword [edi + 56]  ;eflag
    	mov dword [save_eflag],eax

    	mov eax,dword [edi + 32]  ; cs
    	mov dword [save_cs],eax

    	mov eax,dword [edi + 60]  ; eip
    	mov dword [save_eip],eax

    	pop eax
    	pop edi
    	mov edi,dword [save_edi]

    	push dword [save_ss]
    	push dword [save_esp]
    	push dword [save_eflag]
    	push dword [save_cs]
    	push dword [save_eip]

_no:        
    	push dword 0
    	push dword 32
    	pushad
	push ds
	push es
	push ss
	push fs
	push gs
	
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

    	call timer_handler

    	;EOI    
    	mov al,0x20
    	out 0x20,al
   
	pop gs
	pop fs
	pop ss
    	pop es
	pop ds
	popad
	add esp,8
    
	iretd

