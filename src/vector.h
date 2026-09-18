#pragma once

#include <stddef.h>

typedef struct {
  void *data;
  int width;
  int height;
  size_t elem_size;
} Vector;

void vector_resize(Vector *v, int width, int height);
void vector_free(Vector *v);
