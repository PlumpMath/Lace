#pragma once

#include <SDL.h>

int performReload;

extern int quit;
int (*f)(int);
int (*g)();
int *h;

SDL_Point (*updateFn)(SDL_Point);

int render(SDL_Window* win);

