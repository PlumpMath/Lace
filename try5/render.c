#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "hotrod.h"

int quit;

SDL_Point positions[] = { { 100, 200 },
                          { 500, 400 },
                          { 300, 100 },
                          { 130, 100 },
                          { 110, 600 },
                          { 90,  80 },
                          { 200, 300 }};

SDL_Window *render_setup() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("%s\n", SDL_GetError());
        return NULL;
    }

    SDL_Window *win = SDL_CreateWindow("L . A . C . E", 100, 100, 750, 750, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (win == NULL) {
        printf("%s\n", SDL_GetError());
        return NULL;
    }

    int w, h;
    SDL_GL_GetDrawableSize(win, &w, &h);
    printf("Drawable size: %d x %d\n", w, h);

    return win;
}

int render_go(SDL_Window *win) {
    
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
  
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN){
                //quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
        
            }
        }

        // Update
        for(int i = 0; i < sizeof(positions) / sizeof(SDL_Point); i++) {
            SDL_Point p = positions[i];
            if(p.x > 750) { p.x = 0; }
            if(p.x < 0) { p.x = 750; }
            if(p.y > 750) { p.y = 0; }
            if(p.y < 0) { p.y = 750; }
            positions[i] = p;
        }
    
        //Render the scene
        HOTROD(background, renderer);
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
