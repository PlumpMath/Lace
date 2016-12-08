#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

int quit;

void *dylib = NULL;

SDL_Point positions[] = { { 100, 200 },
                          { 500, 400 },
                          { 300, 100 },
                          { 130, 100 },
                          { 110, 600 },
                          { 90,  80 },
                          { 200, 300 }};

void reload() {
  system("clang -g -shared -o f.so f.c `pkg-config --libs sdl2 --cflags sdl2`");
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
  updateFn = dlsym(dylib, "updateFn");

  performReload = 0;
}

int render(SDL_Window *win) {
  
  reload(); // make sure the function is loaded properly first
  
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("%s\n", SDL_GetError());
    return 1;
  }

  int w, h;
  SDL_GL_GetDrawableSize(win, &w, &h);
  printf("Drawable size: %d x %d\n", w, h);

  SDL_RenderSetScale(renderer, 2, 2);
  
  SDL_Surface *bmp = SDL_LoadBMP("./Dot.bmp");
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
      //printf("Will reload.\n");
      reload();
      //printf("Did reload.\n");
    }

    // Update
    for(int i = 0; i < sizeof(positions) / sizeof(SDL_Point); i++) {
      SDL_Point p = updateFn(positions[i]);
      if(p.x > 750) { p.x = 0; }
      if(p.x < 0) { p.x = 750; }
      if(p.y > 750) { p.y = 0; }
      if(p.y < 0) { p.y = 750; }
      positions[i] = p;
    }
    
    //Render the scene
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = { 0, h / 2 - 32, w / 2, 32 };
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_Point *prev = NULL;
    for(int i = 0; i < sizeof(positions) / sizeof(SDL_Point); i++) {
      SDL_Point p = positions[i];
      if(prev) {
        SDL_RenderDrawLine(renderer, prev->x + 32, prev->y + 32, p.x + 32, p.y + 32);
      }
      prev = positions + i;
      SDL_Rect frame = { p.x, p.y, 64, 64 };
      SDL_RenderCopy(renderer, tex, NULL, &frame);
    }
    
    SDL_RenderPresent(renderer);
  }

  return 0;
}
