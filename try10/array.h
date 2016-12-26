#ifndef ARRAY_H
#define ARRAY_H

typedef unsigned int uint;

typedef struct {
    uint count;
    uint allocated_count;
    void **data;
} Array;

Array *array_new(uint prealloc_count);
void array_free(Array *a);
void array_add(Array *a, void *item);
void array_remove(Array *a, uint index);

#endif
