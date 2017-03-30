
CC = gcc
CFLAGS= -g -O2 -Wall -Wundef -pthread
OBJECTS =

all: test.c gz_barrier.c
	$(CC) $(CFLAGS) $^ -o test

clean:
	rm -rf *.out test
