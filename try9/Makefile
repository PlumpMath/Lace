CFLAGS=`pkg-config --cflags sdl2` -I./include
LDFLAGS=`pkg-config --libs sdl2`
FRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit
APPNAME=Landscape

all: landscape.o;
	clang hotrod.c landscape.c glap.c array.c glad.c files.c -o $(APPNAME) -g -O0 $(CFLAGS) $(LDFLAGS) $(FRAMEWORKS)

run:
	./$(APPNAME)

clean: rm -f *.o $(APPNAME)
