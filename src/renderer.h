#include <stdlib.h>

#include "vector.h"

// This 3D renderer is made from
// https://www.youtube.com/watch?v=qjWkNZ0SXfo&list=LL&index=1&t=1084s were
// Tsoding provides a beautiful walkthrough of how to do it.

typedef struct {
  float x, y;
} vec2;

typedef struct {
  float x, y, z;
} vec3;

vec3 screen(vec3 p, int width, int height);
vec3 projection(vec3 p, float aspect);
void render(int width, int height);
void generate_image(int width, int height);
Vector get_image();
