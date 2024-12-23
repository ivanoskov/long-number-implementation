CC=gcc
CFLAGS=-Wall -Wextra -O2

all: factorial

factorial: main.c longnum.c longnum.h
	$(CC) $(CFLAGS) main.c longnum.c -o factorial

clean:
	rm -f factorial
	rm -f *.o

.PHONY: all clean 