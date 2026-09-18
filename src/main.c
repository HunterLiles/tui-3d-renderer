#include "tui.h"

int main() {

  init_terminal();
  Rect rect = {.x = 0,
               .y = 0,
               .width = app.viewport.width,
               .height = app.viewport.height};

  clear_screen();
  while (app.isRunning) {

    draw_screen(); // Draw last completed frame before starting new frame.

    input();

    draw_rect(rect);

    swap_buffer(); // TODO : Possibly resize viewport here?
  };
  restore_terminal();
  return 0;
}
