CFLAGS=`pkg-config --cflags sdl2 freetype2` -I./include
LDFLAGS=`pkg-config --libs sdl2 freetype2`
FRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit
APPNAME=Fonts

all: fonts.o;
	clang fonts.c glap.c glad.c files.c -o $(APPNAME) -g -O0 $(CFLAGS) $(LDFLAGS) $(FRAMEWORKS)

run:
	./$(APPNAME)

clean: rm -f *.o $(APPNAME)
