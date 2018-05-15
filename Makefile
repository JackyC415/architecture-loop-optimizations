CC = gcc -std=gnu99

CFLAGS = -msse4 -O3 -pipe -fno-omit-frame-pointer

all:	bench-naive bench-optimize

#naive
bench-naive: benchmark.o dgemm-naive.o
	$(CC) -o $@ benchmark.o dgemm-naive.o 

#optimize
bench-optimize: benchmark.o dgemm-optimize.o
	$(CC) -o $@ benchmark.o dgemm-optimize.o

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *~ bench-naive bench-optimize *.o
