#include <stdio.h>
#include <stdlib.h>

typedef struct {
  const char *file;
  const int line;
  const size_t size;
  void *mem;
  int freed;
} Allocation;

Allocation allocations[1024];
int last_alloc = 0;

void *malloc_traced(size_t size, char *file, int line) {
  void *memory = malloc(size);
  Allocation a = {
    .file = file,
    .line = line,
    .size = size,
    .mem = memory
  };
  allocations[last_alloc++] = a;
  return memory;
}

void free_traced(void *ptr) {
  for(int i = 0; i < last_alloc; i++) {
    if(allocations[i].mem == ptr) {
      allocations[i].freed = 1;
      break;
    }
  }
  free(ptr);
}

void show_allocs() {
  for(int i = 0; i < last_alloc; i++) {
    Allocation a = allocations[i];
    if(a.freed == 0) {
      printf("%ld bytes of memory not freed. Allocated in '%s' at line %d (%p).\n", a.size, a.file, a.line, a.mem);
    }
  }
}

#define malloc(size) malloc_traced((size), __FILE__, __LINE__);
#define free(size) free_traced(size);

int main() {
  int *ip = malloc(sizeof(int));
  float *fp = malloc(sizeof(float));
  char *sp = malloc(sizeof(char*) * 256);
  //free(ip);
  show_allocs();
}
