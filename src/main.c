#include <termios.h>
#include <unistd.h>

#define ESC "\x1B"
// stdin = 0, stdout = 1, stderr = 2

struct App {
  int isRunning;
} app;

int input(int byte, char input);
void clear_screen();

int main() {

  struct termios start; // original tty settings for resetting after TUI closes
  struct termios tty;

  tcgetattr(0, &start);
  tty = start;

  tty.c_lflag &= ~(ICANON | ECHO);

  tty.c_cc[VMIN] = 0; // 0 makes it auto update, while 1 makes it wait for 1
                      // byte of input to update
  tty.c_cc[VTIME] = 0;

  tcsetattr(0, TCSAFLUSH, &tty); // stdin

  app.isRunning = 1;
  char c;

  clear_screen();
  while (app.isRunning) {
    app.isRunning = input(read(0, &c, 1), c);
  };

  tcsetattr(0, TCSAFLUSH, &start); // stdin

  return 0;
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
  const char setScreen[] = ESC "[2J" // Clear screen
      ESC "[H";                      // Move cursor to top left
  write(1, setScreen, sizeof(setScreen) - 1);
}
