#include "ascii.h"

static const char *ASCII[] = {" ", ".", ":", "-", "=", "+", "*", "#", "%", "@"};

// Take completed buffer from renderer.c. Make an array of symbols that range
// from dark to light. Take the back buffer and render the buffer from
// renderer.c with the symbols in the ASCII array.
