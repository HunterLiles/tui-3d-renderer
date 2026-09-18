#define WIDTH 1280
#define HEIGHT 720

// This 3D renderer is made from
// https://www.youtube.com/watch?v=qjWkNZ0SXfo&list=LL&index=1&t=1084s were
// Tsoding provides a beautiful walkthrough of how to do it.

typedef struct {
  float x, y;
} vec2;

typedef struct {
  float x, y, z;
} vec3;

vec3 screen(vec3 p);
vec3 projection(vec3 p);
float (*render(void))[WIDTH];
