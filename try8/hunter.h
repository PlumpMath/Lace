#ifndef HUNTER_H
#define HUNTER_H

#include <stdio.h>
#include <stdlib.h>

// Hunter Lib - for hunting down memory leaks.
// Inspired by Eskil Steenberg's "How I Program C" talk.
// TODO: Handle realloc and calloc

void *hunter_malloc(size_t size, char *file, int line);
void hunter_free(void *ptr);
void hunter_print_allocations();

#ifdef HUNTER_ENABLED
#define malloc(size) hunter_malloc((size), __FILE__, __LINE__);
#define free(size) hunter_free(size);
#endif

#endif
