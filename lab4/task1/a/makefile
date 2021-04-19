all: exec

exec: link

asm-all:
	nasm -f elf32 lab4_start.s -o lab4_start.o

c-all: 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1a.c -o main.o

link: asm-all c-all
	ld -m elf_i386 lab4_start.o main.o lab4_util.o -o task0
	rm lab4_start.o main.o lab4_util.o