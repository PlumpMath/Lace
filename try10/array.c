#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Array *array_new(uint prealloc_count) {
    Array *a = malloc(sizeof(Array));
    a->count = 0;
    a->allocated_count = prealloc_count;
    a->data = malloc(prealloc_count * sizeof(void*));
    return a;
}

void array_free(Array *a) {
    free(a->data);
    free(a);
}

void array_add(Array *a, void *item) {
    if (a->count == a->allocated_count) {
        a->allocated_count *= 2;
        a->data = realloc(a->data, a->allocated_count * sizeof(void*));
    }
    a->data[a->count++] = item;
}

void array_remove(Array *a, uint index) {
    void *last = a->data[a->count - 1];
    a->data[index] = last;
    a->count--;
}

