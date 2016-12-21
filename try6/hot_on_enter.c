#include "glad.h"
#include "gb_math.h"

extern gbMat4 transform;

void on_enter() {
  gbVec3 up = { 0, 1, 0 };
  gbMat4 rotMat;
  gb_mat4_rotate(&rotMat, up, 0.2);
  gb_mat4_mul(&transform, &rotMat, &transform);
}
