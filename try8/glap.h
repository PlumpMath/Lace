#ifndef GLAP_H
#define GLAP_H

#include "glad.h"
#include <SDL.h>

#define OK(error) if((error) != 0) { fprintf(stderr, "Error: %s\n", SDL_GetError()); exit(1); }

typedef struct {
    SDL_Window *window;
} Glap;

Glap glap_start();
void glap_stop(Glap glap);

GLuint load_shader_program();

#endif
