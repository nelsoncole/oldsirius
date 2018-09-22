ASFLAGS  :=-f elf32
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./lib/include
LDFLAGS  := -Map app.map -T link.ld
AS=nasm
CC=gcc
LD=ld

head	:= head.o
shell	:= shell.o
lib1	:= lib/lib1.a

.PHONY: shell.bin clean

shell.bin: $(head) $(shell) $(lib1)
	$(LD) $(LDFLAGS) -o $@ $^

#head .asm
%.o: head/%.asm
	$(AS) $(ASFLAGS) $< -o $@
#shell .c
%.o: shell/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

mov.bin:
	-mv *.bin ../bin/System/Bin
clean: mov.bin
	rm *.o
