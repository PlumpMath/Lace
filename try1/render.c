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
      if(p.x > 1500) { p.x = 0; }
      if(p.x < 0) { p.x = 1500; }
      if(p.y > 1500) { p.y = 0; }
      if(p.y < 0) { p.y = 1500; }
      positions[i] = p;      
    }
    
    //Render the scene
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    for(int i = 0; i < sizeof(positions) / sizeof(SDL_Point); i++) {
      SDL_Point p = positions[i];
      SDL_Rect frame = { p.x, p.y, 64, 64 };
      SDL_RenderCopy(renderer, tex, NULL, &frame);
    }
    
    SDL_RenderPresent(renderer);
  }

  return 0;
}
