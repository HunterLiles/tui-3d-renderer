#include "renderer.h"
#include "tui.h"

int main() {

  init_terminal();
  Rect rect = {.x = 0,
               .y = 0,
               .width = app.viewport.width,
               .height = app.viewport.height};

  clear_screen(); // Clear once outside so the screen doesn't flicker
  while (app.isRunning) {

    render(app.viewport.width, app.viewport.height);
    generate_image(app.viewport.width, app.viewport.height);

    Vector ascii = get_image();
    const char **back = back_buf.data;
    const char **ascii_data = ascii.data;

    for (int y = 0; y < app.viewport.height; y++)
      for (int x = 0; x < app.viewport.width; x++)
        back[y * back_buf.width + x] = ascii_data[y * ascii.width + x];

    draw_screen(); // Draw last completed frame before starting new frame.

    input();

    draw_rect(rect);

    swap_buffer(); // TODO : Possibly resize viewport here?
  };
  restore_terminal();
  return 0;
}
