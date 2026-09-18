#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define ESC "\x1B"
// stdin = 0, stdout = 1, stderr = 2

typedef struct {
  int width, height;
} Viewport; // Assumed zeros on x and y.

typedef struct {
  int x, y;
  int width, height;
} Rect;

typedef struct {
  int isRunning;
  Viewport viewport;
} App;

extern App app;

void swap_buffer();
void input();
void clear_screen();
void restore_terminal();
void init_terminal();
void draw_screen();
void draw_rect(Rect rect);
