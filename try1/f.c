#include <SDL.h>

SDL_Point updateFn(SDL_Point pos) {
  SDL_Point p = { pos.x + 1, pos.y - 1 };
  return p;
}
