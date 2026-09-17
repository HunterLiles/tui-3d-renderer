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

  tty.c_cc[VMIN] = 1;
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
    }
  }

  char c;

  while (app.isRunning) {
    clear_screen();
    for (int y = 0; y < app.viewport.height; y++) {
      for (int x = 0; x < app.viewport.width; x++) {
        write(1, front_buf[y][x], sizeof(front_buf[y][x]));
      }
    }

    app.isRunning = input(read(0, &c, 1), c);

    draw_rect(rect, app.viewport, back_buf);

    swap_buffer(app.viewport, back_buf, front_buf);
  };

  tcsetattr(0, TCSAFLUSH, &start); // stdin

  return 0;
}

void swap_buffer(Viewport viewport,
                 const char *front_buf[viewport.width][viewport.height],
                 const char *back_buf[viewport.width][viewport.height]) {
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
  const char setScreen[] = ESC "[2J"      // Clear screen
      ESC "[H";                           // Move cursor to top left
  write(1, setScreen, sizeof(setScreen)); // stdout
}

void draw_rect(Rect rect, Viewport viewport,
               const char *buffer[viewport.width][viewport.height]) {
  const char *solid_borders[6] = {"─", "│", "┌", "┐", "└", "┘"};

  for (int y = rect.y; y < rect.height; y++) {
    for (int x = rect.x; x < rect.width; x++) {
      if (y == rect.y || y == rect.height) {
        buffer[y][x] = solid_borders[0];
      } // rows
      if (x == rect.x || x == rect.width) {
        buffer[y][x] = solid_borders[1];
      } // columns
    }
  }
}
