CFLAGS=
LDFLAGS=

all: main.o;
	clang main.c shared.c hotrod.c -o main -O0 $(CFLAGS) $(LDFLAGS)

run:
	./main

clean:
	rm -f *.o hot_*.c hot_*.so main
