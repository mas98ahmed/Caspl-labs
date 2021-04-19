all: exec

exec:
	nasm -f elf32 start.s -o start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o main.o
	ld -m elf_i386 start.o main.o util.o -o task2b
	rm start.o main.o util.o

.PHONY: clean
clean:
	rm -rf ./*o task2c