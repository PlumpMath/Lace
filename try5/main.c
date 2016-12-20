#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <SDL.h>
#include "render.h"
#include "hotrod.h"

int main() {
    hotrod_print_compile_command = true;
    hotrod_extra_includes = (char*[]) { "/usr/local/include/SDL2/SDL.h", NULL };
    SDL_Window *win = render_setup();
    return render_go(win);
}
