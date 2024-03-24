CC=gcc
CFLAGS=-march=native -g3

OFLAGS=-O0

all: dist

dist: dist.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@ -lm

clean:
	rm -Rf *~ dist *.optrpt

