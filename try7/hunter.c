#include "hunter.h"

#undef HUNTER_ENABLED // Must be turned off in this file to be able to use actual 'malloc' and 'free'

typedef struct {
    char *file;
    int line;
    size_t size;
    void *memory;
    int freed;
} Allocation;

static Allocation *allocations;
static int alloc_size = 0;
static int alloc_counter = 0;

void allocations_add(Allocation a) {
    if(alloc_size == 0) {
        alloc_size = 1;
        allocations = malloc(sizeof(Allocation) * alloc_size);
    }
  
    allocations[alloc_counter++] = a;
  
    if(alloc_counter >= alloc_size) {
        alloc_size *= 2;
        allocations = realloc(allocations, sizeof(Allocation) * alloc_size);
    }
}

void *hunter_malloc(size_t size, char *file, int line) {
    void *memory = malloc(size);
    Allocation a = {
        .file = file,
        .line = line,
        .size = size,
        .memory = memory
    };
    allocations_add(a);
    return memory;
}

void hunter_free(void *ptr) {
    for(int i = 0; i < alloc_counter; i++) {
        if(allocations[i].memory == ptr) {
            allocations[i].freed = 1;
            break;
        }
    }
    free(ptr);
}

void hunter_print_allocations() {
    printf("~ Hunter (%d registered allocations) ~\n", alloc_counter);
    for(int i = 0; i < alloc_counter; i++) {
        Allocation a = allocations[i];
        if(a.freed == 0) {
            printf("%ld bytes of memory not freed (%p). Allocated in '%s' at line %d.\n", a.size, a.memory, a.file, a.line);
        }
    }
}

