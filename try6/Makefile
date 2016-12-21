CFLAGS=`pkg-config --cflags sdl2` -I./include
LDFLAGS=`pkg-config --libs sdl2`
FRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit

all: main.o;
	clang main.c glad.c hotrod.c files.c hunter.c -o Try6 -g -O0 $(CFLAGS) $(LDFLAGS) $(FRAMEWORKS)

run:
	./Try6

clean: rm -f *.o Try6
