#ifndef CHIP8KEYBOARD_HPP
#define CHIP8KEYBOARD_HPP

#include "chip8.hpp"
#include <raylib.h>

void keyboard(Chip8* subject)
{ 
    subject->keyboard[0x1] = IsKeyDown(KEY_1);
    subject->keyboard[0x2] = IsKeyDown(KEY_2);
    subject->keyboard[0x3] = IsKeyDown(KEY_3);
    subject->keyboard[0xC] = IsKeyDown(KEY_4);

    subject->keyboard[0x4] = IsKeyDown(KEY_Q);
    subject->keyboard[0x5] = IsKeyDown(KEY_W);
    subject->keyboard[0x6] = IsKeyDown(KEY_E);
    subject->keyboard[0xD] = IsKeyDown(KEY_R);

    subject->keyboard[0x7] = IsKeyDown(KEY_A);
    subject->keyboard[0x8] = IsKeyDown(KEY_S);
    subject->keyboard[0x9] = IsKeyDown(KEY_D);
    subject->keyboard[0xE] = IsKeyDown(KEY_F);

    subject->keyboard[0xA] = IsKeyDown(KEY_Z);
    subject->keyboard[0x0] = IsKeyDown(KEY_X);
    subject->keyboard[0xB] = IsKeyDown(KEY_C);
    subject->keyboard[0xF] = IsKeyDown(KEY_V);
}

#endif
