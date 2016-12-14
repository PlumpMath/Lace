#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <assert.h>

char *hotrod_prefix = "hot_";

void hotrod_ensure_func(const char *fn_name, void **dylib_ptr, void **fn_ptr) {
  char *filename;
  char *dylib_name;
  asprintf(&filename, "%s%s.c", hotrod_prefix, fn_name);
  asprintf(&dylib_name, "%s.so", fn_name);

  // Ensure file
  FILE *file = fopen(filename, "r");
  if(!file) {
    printf("Creating file %s\n", filename);
    file = fopen(filename, "w");
    assert(file);
    fprintf(file, "void %s() {\n    printf(\"%s\\n\");\n}\n", fn_name, fn_name);
  }
  fclose(file);

  // Compile code
  char *compile_cmd;
  asprintf(&compile_cmd, "clang -include stdio.h -shared -o %s.so %s", fn_name, filename);
  system(compile_cmd);

  // Load function from library 
  if(*dylib_ptr) {
    dlclose(*dylib_ptr);
  }
  *dylib_ptr = dlopen(dylib_name, RTLD_LAZY);
  if(!*dylib_ptr) {
    fprintf (stderr, "%s\n", dlerror());
    exit(1);
  }
  
  *fn_ptr = dlsym(*dylib_ptr, fn_name);

  // Cleanup
  free(filename);
  free(dylib_name);
}

#define HOTROD(name)                                              \
  void *name##_lib = NULL;                                        \
  void (*name##_fn)() = NULL;                                     \
  hotrod_ensure_func(#name, &name##_lib, (void*)&name##_fn);      \
  name##_fn();

int main() {
  HOTROD(bleh)
}

