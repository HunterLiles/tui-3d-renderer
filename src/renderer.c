#include "renderer.h"

float buffer[HEIGHT][WIDTH];

// TODO : Somehow I need to take in a mesh and not have a predefined one.

vec3 screen(vec3 p) {
  return (vec3){.x = (p.x + 1) / 2 * WIDTH, .y = (1 - (p.y + 1) / 2) * HEIGHT};
}

vec3 projection(vec3 p) {
  return (vec3){
      .x = p.x / p.z,
      .y = p.y / p.z,
  };
}

vec3 translation(vec3 p, vec3 translate) {
  return (vec3){
      .x = p.x + translate.x,
      .y = p.y + translate.y,
      .z = p.z + translate.z,
  };
}

float (*render(void)) [WIDTH] {
  vec3 mesh[] = {
      {.x = 0.5, .y = 0.5, .z = 1},
      {.x = -0.5, .y = 0.5, .z = 1},
      {.x = 0.5, .y = -0.5, .z = 1},
      {.x = -0.5, .y = -0.5, .z = 1},
  };
  vec3 move = {.x = 0, .y = 0, .z = 1};
  int num_rows = sizeof(mesh) / sizeof(mesh[0]);

  for (int i = 0; i < num_rows; i++) {
    vec3 p = screen(projection(translation(mesh[i], move)));

    buffer[(int)p.y][(int)p.x] = 1.0f; // Final rendered output.
  }
  return buffer;
}
