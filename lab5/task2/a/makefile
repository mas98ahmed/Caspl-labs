all: exec

test:
	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32  looper.o -o looper
	rm looper.o

exec: task2a.c
	gcc -g -m32 -c -o myshell.o task2a.c
	gcc -g -m32  myshell.o -o task
	rm myshell.o


clean:
	rm -rf ./*.o task
