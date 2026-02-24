#include "chip8.hpp"
#include "chip8Instructions.hpp"
#include "chip8Keyboard.hpp"
#include "dispatcher.hpp"
#include "screenDraw.hpp"
#include "loadRom.hpp"

#include <iostream>
#include <raylib.h>
#include <cstring>

extern const int g_size { 20 };
extern const Color g_font_color { BLACK };

int main(int argc, char* argv[]) 
{ 
    if (argc <= 1) 
    { 
        std::cout << "usage: " << argv[0] << " /path/to/rom\n";
        return 1;
    }

    Chip8 machine {};

    memcpy(machine.Memory, chip8Font, sizeof(chip8Font));
    machine.PC = 0x200;

    InitWindow(64 * g_size, 32 * g_size, "Chippy");
    SetTargetFPS(60);

    loadRom(&machine, argv[1]);
    while(!WindowShouldClose())
    { 
        keyboard(&machine);

        if (machine.DT > 0) --machine.DT;
        if (machine.ST > 0) --machine.ST;

        for (int i = 0; i < 10; ++i)
        { 
            uint16_t opcode = (machine.Memory[machine.PC] << 8) | machine.Memory[machine.PC + 1];
            machine.PC += 2;
            execute(&machine, opcode);
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawInRayLib(&machine);
        EndDrawing();
    }
    return 0; 
}
