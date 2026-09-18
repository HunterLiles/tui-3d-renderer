#include "vector.h"

#include <stdlib.h>

void vector_resize(Vector *v, int width, int height) {
  if (v->width == width && v->height == height)
    return;

  free(v->data);
  v->data = calloc((size_t)width * (size_t)height, v->elem_size);
  v->width = width;
  v->height = height;
}

void vector_free(Vector *v) {
  free(v->data);
  v->data = NULL;
  v->width = 0;
  v->height = 0;
}
