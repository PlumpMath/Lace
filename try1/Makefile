CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib/
LDLIBS=-lsdl2

all: main.o;
	clang main.c render.c -o Lace -g -O0 $(CFLAGS) $(LDFLAGS) $(LDLIBS)

run:
	./Lace

clean: rm -f *.o main
