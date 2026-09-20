# 3D Terminal Renderer

This project uses C with no external dependencies to create a TUI and then draw 3D images to the TUI in ASCII art.

The TUI is made using termios and ANSI codes. The 3D renderer is made by following [Tsoding](https://www.youtube.com/@Tsoding) on youtube where he shows the power of x/z and y/z in his video "One Formula That Demistifies 3D Grahpics".

I had to recreate vectors in C for an easier time with the buffers as normal arrays required me to constantly pass around width and height and I was having trouble making the buffers because in order to allow for resizing of the terminal window I can not have fixed sized buffers, the simplest way I deemed to allow for dynamic length was to recreate vectors.
