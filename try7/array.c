#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HUNTER_ENABLED
#include "hunter.h"

typedef unsigned int uint;

typedef struct {
    uint count;
    uint allocated_count;
    void **data;
} Array;

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

int main() {
    Array *a = array_new(3);
    array_add(a, "Hej");
    array_add(a, "san");
    array_add(a, "på");
    array_add(a, "dig");
    array_add(a, "!");
    for (int i = 0; i < a->count; i++) {
        printf("%s ", a->data[i]);
    }
    printf("\n");
    assert(a->allocated_count == 6); // should have doubled once
    array_remove(a, 1);
    for (int i = 0; i < a->count; i++) {
        printf("%s ", a->data[i]);
    }
    array_add(a, "?");
    array_add(a, "???");
    assert(a->allocated_count == 6);
    array_add(a, "?!");
    assert(a->allocated_count == 12);
    printf("\n");
    array_free(a);
    hunter_print_allocations();
}
