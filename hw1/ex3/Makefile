
CC = gcc
CFLAGS= -g -O2 -Wall -Wundef -pthread
OBJECTS =

all: test naivetest

test: test.c gz_barrier.c
	$(CC) $(CFLAGS) $^ -o test

naivetest: naive_test.c gz_barrier.c
	$(CC) $(CFLAGS) $^ -o naive_test

clean:
	rm -rf *.out test naive_test
