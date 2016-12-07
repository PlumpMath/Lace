#pragma once

#include <SDL2/SDL.h>

extern int quit;
int (*f)(int);
int (*g)();
int *h;

int render(SDL_Window* win);

