AS=nasm
.PHONY: fatboot.bin cdboot.bin move.bin

#compiler fatboot
fatboot.bin: fatboot.asm 
	$(AS) -f bin -o $@ $<

#compiler cdboot
cdboot.bin: cdboot.asm 
	$(AS) -f bin -o $@ $<

#move binary
move.bin:
	mv *.bin ../../bin/boot/sirius


