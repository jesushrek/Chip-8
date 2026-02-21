#ifndef SCREENDRAW_HPP
#define SCREENDRAW_HPP
#include "chip8.hpp"
#include <raylib.h>

extern const int g_size;

void DrawInRayLib(Chip8* subject)
{ 
    for (int i = 0; i < 32; ++i)
    { 
        for (int j = 0; j < 64; ++j)
        { 
            if (subject->screen[j * 64 + i])
                DrawRectangle(j * g_size, i * g_size, g_size, g_size, WHITE);
        }
    }
}

#endif
