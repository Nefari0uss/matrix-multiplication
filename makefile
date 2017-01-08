# makefile for custom unix shell
all: matrix-multiplication.c
	gcc matrix-multiplication.c -o mm -Wall -lpthread -lrt -O3 
clean:
	$(RM) mm

