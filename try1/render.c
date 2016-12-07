#include "render.h"

int quit;

int render(SDL_Window *win) {
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

    SDL_Rect frame = { 100, 100, 405, 550 };

    //Render the scene
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, &frame);
    SDL_RenderPresent(renderer);
  }

  return 0;
}
