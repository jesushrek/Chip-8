#ifndef SCREENDRAW_HPP
#define SCREENDRAW_HPP
#include "chip8.hpp"
#include <raylib.h>

extern const int g_size;
extern const Color g_font_color;

void DrawInRayLib(Chip8* subject)
{ 
    for (int i = 0; i < 32; ++i)
    { 
        for (int j = 0; j < 64; ++j)
        { 
            if (subject->screen[i * 64 + j])
                DrawRectangle(j * g_size, i * g_size, g_size, g_size, g_font_color);
        }
    }
}

#endif
