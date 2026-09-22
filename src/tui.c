#include "tui.h"

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

App app;
struct termios start;
struct termios tty;

static Vector front_buf = {.elem_size = sizeof(const char *)};
Vector back_buf = {.elem_size = sizeof(const char *)};

void init_terminal() {

  tcgetattr(0, &start);
  tty = start;

  tty.c_lflag &= ~(ICANON | ECHO);

  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 0;

  tcsetattr(0, TCSAFLUSH, &tty); // stdin

  struct winsize size;
  ioctl(0, TIOCGWINSZ, &size);

  app = (App){.isRunning = 1,
              .viewport = {.width = size.ws_col, .height = size.ws_row}};

  vector_resize(&front_buf, app.viewport.width, app.viewport.height);
  vector_resize(&back_buf, app.viewport.width, app.viewport.height);

  const char **front = front_buf.data;
  const char **back = back_buf.data;

  for (int y = 0; y < app.viewport.height; y++) {
    for (int x = 0; x < app.viewport.width; x++) {
      front[y * app.viewport.width + x] = " ";
      back[y * app.viewport.width + x] = " ";
    }
  }
}

void swap_buffer() {
  const char **front = front_buf.data;
  const char **back = back_buf.data;

  for (int y = 0; y < app.viewport.height; y++) {
    for (int x = 0; x < app.viewport.width; x++) {
      front[y * app.viewport.width + x] = back[y * app.viewport.width + x];
      back[y * app.viewport.width + x] = " ";
    }
  }
}

void input() {
  char c;
  int byte = read(0, &c, 1);

  if (byte > 0) {
    switch (c) {
    case 'q':
      app.isRunning = 0;
    default:
      break;
    }
  }
}

void clear_screen() {
  const char set[] = ESC "[?1049h" ESC "[?25l" ESC "[?7l" ESC "[2J" ESC "[H";
  write(1, set, sizeof(set)); // stdout
}

void restore_terminal() {
  const char restore[] = ESC "[?7h" ESC "[?25h" ESC "[?1049l";
  write(1, restore, sizeof(restore));

  tcsetattr(0, TCSAFLUSH, &start); // stdin

  vector_free(&front_buf);
  vector_free(&back_buf);
}

void draw_rect(Rect rect) {
  static const char *SOLID_BORDERS[6] = {"─", "│", "┌", "┐", "└", "┘"};
  const char **back = back_buf.data;

  for (int y = rect.y; y < rect.height; y++) {
    for (int x = rect.x; x < rect.width; x++) {
      if (y == rect.y || y == rect.y + rect.height - 1)
        back[y * back_buf.width + x] = SOLID_BORDERS[0];
      if (x == rect.x || x == rect.x + rect.width - 1)
        back[y * back_buf.width + x] = SOLID_BORDERS[1];
      if (x == rect.x && y == rect.y)
        back[y * back_buf.width + x] = SOLID_BORDERS[2];
      if (x == rect.x + rect.width - 1 && y == rect.y)
        back[y * back_buf.width + x] = SOLID_BORDERS[3];
      if (x == rect.x && y == rect.y + rect.height - 1)
        back[y * back_buf.width + x] = SOLID_BORDERS[4];
      if (x == rect.x + rect.width - 1 && y == rect.y + rect.height - 1)
        back[y * back_buf.width + x] = SOLID_BORDERS[5];
    }
  }
}

void draw_screen() {
  write(1, "\x1b[H", 3);
  const char **front = front_buf.data;

  for (int y = 0; y < app.viewport.height; y++) {
    char pos[32];

    int len = snprintf(pos, sizeof(pos), ESC "[%d;1H", y + 1);
    write(1, pos, len);

    for (int x = 0; x < app.viewport.width; x++) {
      const char *cell = front[y * front_buf.width + x];
      write(1, cell, strlen(cell));
    }
  }
}
