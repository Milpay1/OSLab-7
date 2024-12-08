all: matrix

matrix: matrix.c
	gcc -std=c99 -pthread -o matrix matrix.c -I.

clean:
	rm -f matrix
