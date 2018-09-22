ASFLAGS  :=-f elf32
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc -fno-align-functions -I ./include
LDFLAGS  := -Map Kernel.map -T link.ld
AS=nasm
CC=gcc
LD=ld

microkernel 	:= setup.o flush.o main.o io.o gdt.o ldt.o tss.o idt.o isrs.o exceptions.o irqs.o irq0.o task.o system.o  string.o
executive	:= heap.o page.o mm.o syscall.o int69.o execve.o vfs.o sfs.o iso9660.o part.o
drivers 	:= pic.o console.o pci.o pit.o ps2.o keyboard.o mouse.o rtc.o ata.o ata_pci.o ide.o ahci.o ehci.o hda.o vbe.o gpu.o i82540.o

gui		:= gui.o pixel.o drawchar.o font_8x8.o

.PHONY: kernel.bin clean move.bin

kernel.bin: $(microkernel) $(executive) $(drivers) $(gui)
	$(LD) $(LDFLAGS) -o $@ $^ 
#microkernel .asm
%.o: microkernel/%.asm
	$(AS) $(ASFLAGS) $< -o $@

#microkernel  .c
%.o: microkernel/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#executive .c
%.o: executive/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#executive/memory .c
%.o: executive/memory/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#executive/syscall .asm
%.o: executive/syscall/%.asm
	$(AS) $(ASFLAGS) $< -o $@
#executive/syscall .c
%.o: executive/syscall/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#executive/fs .c
%.o: executive/fs/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers .c
%.o: drivers/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#drivers/pit .c
%.o: drivers/pit/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#drivers/ps2 .c
%.o: drivers/ps2/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/rtc .c
%.o: drivers/rtc/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/ata .c
%.o: drivers/ata/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/audio .c
%.o: drivers/audio/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/video .c
%.o: drivers/video/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/video/vbe .c
%.o: drivers/video/vbe/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#drivers/usb .c
%.o: drivers/usb/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
#drivers/net/i82540 .c
%.o: drivers/net/i82540/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

#gui .c
%.o: gui/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

move.bin:
	-mv *.bin ../bin/Kernel

clean: move.bin
	rm *.o
