#include <SDL.h>

int f(int x) {
  return 0;
}

int g() {
  return 550;
}

//int h = 440;

SDL_Point updateFn(SDL_Point pos) {
  SDL_Point p = { pos.x + 1, pos.y - 10 };
  return p;
}
