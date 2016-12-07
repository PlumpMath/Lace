CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib/
LDLIBS=-lsdl2 -framework Cocoa -framework IOKit

all: main.o;
	clang main.c render.c -o Lace $(CFLAGS) $(LDFLAGS) $(LDLIBS)

run:
	./Lace

clean: rm -f *.o main
