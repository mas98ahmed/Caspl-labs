all: exec

exec: b.c
	gcc -g -m32 -Wall -c -o b.o b.c
	gcc -g -m32 -Wall -o b b.o
	rm b.o

clean:
	rm b