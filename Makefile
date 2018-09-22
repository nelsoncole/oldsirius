PWD := $(shell pwd)

.PHONY: mount test mountcd


mount:
	#make -f boot/Makefile
	#@[make -f kernel/Makefile]
#criando a imagem
	#dd of=Sirius.raw if=/dev/zero bs=1024 count=32768
	qemu-img create -f raw Sirius.raw 32M
#montando o imagem
	gcc -C util/kholefs.c -o kholefs
	./kholefs bin/Boot/Sirius/fatboot.bin Sirius.raw
	sudo mount Sirius.raw /mnt/
	sudo mkdir /mnt/lib/
	sudo mkdir /mnt/system/
	sudo mkdir /mnt/system/bin/
	sudo mkdir /mnt/system/boot/
	sudo mkdir /mnt/system/kernel/
	sudo mkdir /mnt/tmp/
	sudo mkdir /mnt/user/
	sudo mkdir /mnt/user/name/
	sudo mkdir /mnt/user/name/desktop/
	sudo mkdir /mnt/user/name/documents/
	sudo mkdir /mnt/user/name/downloads/
	sudo mkdir /mnt/user/name/music/
	sudo mkdir /mnt/user/name/pictures/
	sudo mkdir /mnt/user/name/videos/
	sudo cp bin/Boot/Sirius/stage2.bin /mnt/
	sudo cp bin/Kernel/kernel.bin /mnt/
	#sudo cp user/bin/shell.bin /mnt/system/bin/
	#sudo cp bin/boot/Sirius/boot.cfg /mnt/
	sudo umount /mnt/
mountcd:
	mkisofs -R -J -c Boot/boot.catalog -b Boot/Sirius/cdboot.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o Sirius.iso bin

#testing 	
test:
	qemu-system-i386 -m 64 -drive file=Sirius.raw,index=0,if=ide,format=raw -boot order=d,menu=on -cdrom Sirius.iso
	#rm -v khole.raw
