#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define ESC "\x1B"
// stdin = 0, stdout = 1, stderr = 2

typedef struct {
  int width, height;
} Viewport;

typedef struct {
  int x, y;
  int width, height;
} Rect;

typedef struct {
  int isRunning;
  Viewport viewport;
} App;

int input(int byte, char input);
void clear_screen();
void restore_screen();
void draw_rect(Rect rect, Viewport viewport,
               const char *buffer[viewport.height][viewport.width]);
void swap_buffer(Viewport viewport,
                 const char *back_buf[viewport.height][viewport.width],
                 const char *front_buf[viewport.height][viewport.width]);

int main() {

  struct termios start;
  struct termios tty;

  tcgetattr(0, &start);
  tty = start;

  tty.c_lflag &= ~(ICANON | ECHO);

  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 0;

  tcsetattr(0, TCSAFLUSH, &tty); // stdin

  struct winsize size;
  ioctl(0, TIOCGWINSZ, &size);

  App app;
  app.isRunning = 1;
  app.viewport.width = size.ws_col;
  app.viewport.height = size.ws_row;

  Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.width = app.viewport.width;
  rect.height = app.viewport.height;

  const char *front_buf[app.viewport.height][app.viewport.width];
  const char *back_buf[app.viewport.height][app.viewport.width];

  for (int y = 0; y < app.viewport.height; y++) {
    for (int x = 0; x < app.viewport.width; x++) {
      back_buf[y][x] = " ";
      front_buf[y][x] = " ";
    }
  }

  char c;

  clear_screen();
  while (app.isRunning) {

    write(1, "\x1b[H", 3);

    for (int y = 0; y < app.viewport.height; y++) {
      char pos[32];

      int len = snprintf(pos, sizeof(pos), ESC "[%d;1H", y + 1);
      write(1, pos, len);

      for (int x = 0; x < app.viewport.width; x++) {
        write(1, front_buf[y][x], strlen(front_buf[y][x]));
      }
    }

    int byte = read(0, &c, 1);
    app.isRunning = input(byte, c);

    draw_rect(rect, app.viewport, back_buf);

    swap_buffer(app.viewport, front_buf, back_buf);
  };

  restore_screen();
  tcsetattr(0, TCSAFLUSH, &start); // stdin

  return 0;
}

void swap_buffer(Viewport viewport,
                 const char *front_buf[viewport.height][viewport.width],
                 const char *back_buf[viewport.height][viewport.width]) {
  for (int y = 0; y < viewport.height; y++) {
    for (int x = 0; x < viewport.width; x++) {
      front_buf[y][x] = back_buf[y][x];
    }
  }
}

int input(int byte, char c) {
  if (byte > 0) {
    switch (c) {
    case 'q':
      return 0;
    default:
      break;
    }
  }
  return 1;
}

void clear_screen() {
  const char set[] = ESC "[?1049h" ESC "[?25l" ESC "[?7l" ESC "[2J" ESC "[H";
  write(1, set, sizeof(set)); // stdout
}

void restore_screen() {
  const char restore[] = ESC "[?7h" ESC "[?25h" ESC "[?1049l";
  write(1, restore, sizeof(restore));
}

void draw_rect(Rect rect, Viewport viewport,
               const char *buffer[viewport.height][viewport.width]) {
  const char *solid_borders[6] = {"─", "│", "┌", "┐", "└", "┘"};

  for (int y = rect.y; y < rect.height; y++) {
    for (int x = rect.x; x < rect.width; x++) {
      if (y == rect.y || y == rect.y + rect.height - 1)
        buffer[y][x] = solid_borders[0];
      if (x == rect.x || x == rect.x + rect.width - 1)
        buffer[y][x] = solid_borders[1];
      if (x == rect.x && y == rect.y)
        buffer[y][x] = solid_borders[2];
      if (x == rect.x + rect.width - 1 && y == rect.y)
        buffer[y][x] = solid_borders[3];
      if (x == rect.x && y == rect.y + rect.height - 1)
        buffer[y][x] = solid_borders[4];
      if (x == rect.x + rect.width - 1 && y == rect.y + rect.height - 1)
        buffer[y][x] = solid_borders[5];
    }
  }
}
