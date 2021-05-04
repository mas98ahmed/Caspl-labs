all: exec

test:
	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32  looper.o -o looper
	rm looper.o

exec: myshell.c test
	gcc -g -m32 -c -o myshell.o myshell.c
	gcc -g -m32  myshell.o -o myshell
	rm myshell.o


clean:
	rm -rf ./*.o myshell
