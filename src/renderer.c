#include "renderer.h"

static Vector render_buf = {.elem_size = sizeof(float)};
static Vector ascii_buf = {.elem_size = sizeof(const char *)};
static const char *ASCII[] = {" ", ".", ":", "-", "=", "+", "*", "#", "%", "@"};

// TODO : Somehow I need to take in a mesh and not have a predefined one.

vec3 screen(vec3 p, int width, int height) {
  return (vec3){.x = (p.x + 1) / 2 * (width - 1),
                .y = (1 - (p.y + 1) / 2) * (height - 1)};
}

vec3 projection(vec3 p, float aspect) {
  return (vec3){
      .x = p.x / p.z / aspect,
      .y = p.y / p.z / aspect,
  };
}

vec3 translation(vec3 p, vec3 translate) {
  return (vec3){
      .x = p.x + translate.x,
      .y = p.y + translate.y,
      .z = p.z + translate.z,
  };
}

// TODO : How can I mathematically draw a line between the points and set a
// light value of 0.5f to them? (0.5f light value to distinguish between the
// points and the lines)

void render(int width, int height) {
  vector_resize(&render_buf, width, height);
  float *buf = render_buf.data;

  vec3 mesh[] = {
      {.x = -0.5, .y = 0.5, .z = 0.5},  {.x = -0.5, .y = 0.5, .z = 1},
      {.x = 0.5, .y = 0.5, .z = 0.5},   {.x = 0.5, .y = 0.5, .z = 1},
      {.x = -0.5, .y = -0.5, .z = 0.5}, {.x = -0.5, .y = -0.5, .z = 1},
      {.x = 0.5, .y = -0.5, .z = 0.5},  {.x = 0.5, .y = -0.5, .z = 1},
  };
  vec3 move = {.x = 0, .y = 0, .z = 1};
  int num_rows = sizeof(mesh) / sizeof(mesh[0]);
  float aspect = (float)width / height;

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      buf[y * width + x] = 0.0f;

  for (int i = 0; i < num_rows; i++) {
    vec3 p =
        screen(projection(translation(mesh[i], move), aspect), width, height);
    int px = (int)p.x;
    int py = (int)p.y;

    if (px >= 0 && px < width && py >= 0 && py < height)
      buf[py * width + px] = 1.0f;
  }
}

void generate_image(int width, int height) {
  vector_resize(&ascii_buf, width, height);

  float *render = render_buf.data;
  const char **ascii = ascii_buf.data;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (render[y * width + x] == 1.0f) {
        ascii[y * width + x] = ASCII[9];
      } else {
        ascii[y * width + x] = ASCII[0];
      }
    }
  }
}

Vector get_image() { return ascii_buf; }
