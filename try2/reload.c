#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <assert.h>

void *dylib = NULL;
void *fn = NULL;

void ensure_reloadable_function(const char *fn_name) {
  char *filename;
  char *dylib_name;
  asprintf(&filename, "%s.c", fn_name);
  asprintf(&dylib_name, "%s.so", fn_name);

  // Ensure file
  FILE *file = fopen(filename, "r");
  if(!file) {
    printf("Creating file %s\n", filename);
    file = fopen(filename, "w");
    assert(file);
    fprintf(file, "void %s() {\n    \n}\n", fn_name);
  }

  // Compile code
  char *compile_cmd;
  asprintf(&compile_cmd, "clang -shared -o %s.so %s", fn_name, filename);
  system(compile_cmd);

  // Load function from library 
  if(dylib) {
    dlclose(dylib);
  }
  dylib = dlopen(dylib_name, RTLD_LAZY);
  if(!dylib) {
    fprintf (stderr, "%s\n", dlerror());
    exit(1);
  }
  
  fn = dlsym(dylib, fn_name);

  // Cleanup
  free(filename);
  free(dylib_name);
}

int main() {
  ensure_reloadable_function("foo");
}

