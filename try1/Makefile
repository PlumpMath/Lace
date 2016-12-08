CFLAGS=`pkg-config --cflags sdl2`
LDFLAGS=`pkg-config --libs sdl2`

all: main.o;
	clang main.c render.c -o Lace -g -O0 $(CFLAGS) $(LDFLAGS)

run:
	./Lace

clean: rm -f *.o main
