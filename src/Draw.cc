#include "Chip8.hh"

#include <raylib.h>

extern const int g_size;
extern const Color g_font_color;

void draw(Chip8 &subject) { 
    for (int i = 0; i < 64; ++i) { 
        for (int j = 0; j < 32; ++j) { 
            if (subject.screen[i])
                DrawRectangle(j * g_size, j * g_size, g_size, g_size, g_font_color);
        }
    }
}
