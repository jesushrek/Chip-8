#include "Chip8.hh"
#include <raylib.h>

void Keypad(Chip8 &subject) 
{ 
    subject.keyPad[0x01] = IsKeyDown(KEY_ONE);
    subject.keyPad[0x02] = IsKeyDown(KEY_TWO);
    subject.keyPad[0x03] = IsKeyDown(KEY_THREE);
    subject.keyPad[0x0C] = IsKeyDown(KEY_FOUR);

    subject.keyPad[0x04] = IsKeyDown(KEY_Q);
    subject.keyPad[0x05] = IsKeyDown(KEY_W);
    subject.keyPad[0x06] = IsKeyDown(KEY_E);
    subject.keyPad[0x0D] = IsKeyDown(KEY_R);

    subject.keyPad[0x07] = IsKeyDown(KEY_A);
    subject.keyPad[0x08] = IsKeyDown(KEY_S);
    subject.keyPad[0x09] = IsKeyDown(KEY_D);
    subject.keyPad[0x0E] = IsKeyDown(KEY_F);

    subject.keyPad[0x0A] = IsKeyDown(KEY_Z);
    subject.keyPad[0x00] = IsKeyDown(KEY_X);
    subject.keyPad[0x0B] = IsKeyDown(KEY_C);
    subject.keyPad[0x0F] = IsKeyDown(KEY_V);
}
