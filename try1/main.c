#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "render.h"

void *dylib = NULL;

void reload() {
  system("clang -shared -o f.so f.c");
  if(dylib) {
    dlclose(dylib);
  }
  dylib = dlopen("f.so", RTLD_LAZY);
  if(!dylib) {
    fputs (dlerror(), stderr);
    exit(1);
  }
  f = dlsym(dylib, "f");
  g = dlsym(dylib, "g");
  h = dlsym(dylib, "h");
}

static int repl(void *ptr) {
  int run = 1;
  while(run) {
    printf("> ");
    char input[1024];
    fgets(input, 1024, stdin);

    if(strcmp(input, "q\n") == 0) {
      run = 0;
      quit = 1;
    }
    else if(strcmp(input, "r\n") == 0) {
      reload();
    }
    else {
      printf("Can't understand command: %s\n", input);
    }    
  }
  return 0;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("%s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow("L A C E", 100, 100, 750, 750, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    printf("%s\n", SDL_GetError());
    return 1;
  }

  SDL_Thread *thread = SDL_CreateThread(repl, "repl", NULL);

  reload(); // make sure the function is loaded properly first
  
  int result = render(win);
  
  SDL_Quit();
  return result;
}
