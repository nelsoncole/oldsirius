CFLAGS=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: all

all:
	gcc ${CFLAGS} -c *.c
	ar -rcs lib1.a *.o
#mov obj
	mv *.o obj
