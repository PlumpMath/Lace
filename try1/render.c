#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

int quit;

void *dylib = NULL;

void reload() {
  system("clang -g -shared -o f.so f.c");
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

  printf("Done\n");
  performReload = 0;
}

int render(SDL_Window *win) {
  
  reload(); // make sure the function is loaded properly first
  
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("%s\n", SDL_GetError());
    return 1;
  }
  
  SDL_Surface *bmp = SDL_LoadBMP("./flower.bmp");
  if (bmp == NULL) {
    printf("%s\n", SDL_GetError());
    return 1;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
  SDL_FreeSurface(bmp);
  if (tex == NULL) {
    printf("%s\n", SDL_GetError());
    return 1;
  }

  SDL_Event e;
  
  while (!quit){
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
        quit = 1;
      }
      if (e.type == SDL_KEYDOWN){
        //quit = 1;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN){
        
      }
    }

    if(performReload) {
      printf("Will reload.\n");
      reload();
      printf("Did reload.\n");
    }

    SDL_Rect frame = { 100, f(10), 440, g() };

    //Render the scene
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, &frame);
    SDL_RenderPresent(renderer);
  }

  return 0;
}
