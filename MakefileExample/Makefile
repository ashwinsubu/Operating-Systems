all: hello

hello: main.o square.o quadratic.o 
	gcc main.o square.o quadratic.o -o test -lm

main.o: main.c
	gcc -c main.c

square.o: square.c
	gcc -c square.c

quadratic.o: quadratic.c
	gcc -c quadratic.c

clean:
	rm -rf *o hello
