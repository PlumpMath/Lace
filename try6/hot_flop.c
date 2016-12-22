#include "glad.h"
#include "gb_math.h"

void flop(gbMat4 *projection) {
  glClearColor(0.9, 0.6, 0.8, 1);

  gb_mat4_identity(projection);
  gb_mat4_perspective(projection, 45, 1.0, 0.1, 100.0);
  //void gb_mat4_perspective(gbMat4 *out, float fovy, float aspect, float z_near, float z_far);
  //gb_mat4_identity(&projection);
  //projection->e[3] = 0.7;
  /* printf("Proj:\n"); */
  /* print_mat4(&projection); */
}
