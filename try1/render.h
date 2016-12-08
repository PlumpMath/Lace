#pragma once

#include <SDL.h>

int quit;
int performReload;

SDL_Window *render_setup();
int render_go(SDL_Window* win);

SDL_Point (*updateFn)(SDL_Point);
