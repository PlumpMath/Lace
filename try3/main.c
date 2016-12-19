#include "hotrod.h"
#include "shared.h"

int main() {
  hotrod_print_compile_command = true;
  HOTROD(f);
  printf("x = %d\n", x);
}
