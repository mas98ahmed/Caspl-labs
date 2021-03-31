all: exec

exec: a.c
	gcc -g -m32 -Wall -c -o a.o a.c
	gcc -g -m32 -Wall -o a a.o
	rm a.o

clean:
	rm a