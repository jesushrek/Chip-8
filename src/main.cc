#include "Chip8.hh"
#include "Draw.cc"
#include "Instructions.cc"
#include "KeyPad.hh"
#include "LoadRom.cc"

#include <algorithm>
#include <iostream>
#include <cstring>

#include <raylib.h>

constexpr int g_width {64};
constexpr int g_height {32};
constexpr int g_size {10};
constexpr Color g_font_color {BLACK};

int main(int argc, char** argv) 
{ 
    if (argc <= 1) { 
        std::cout << "usage: " << argv[0] << " /path/to/rom\n";
        return 0;
    }

    Chip8 machine{};
    std::memcpy(machine.memory, Chip8Font, sizeof(Chip8Font));
    loadRom(machine, argv[1]);
    machine.PC = 0x200;

    InitWindow(g_width * g_size, g_height * g_size, "Chip-8");
    SetTargetFPS(60);
    while (!WindowShouldClose()) { 
        for (int i = 0; i < 10; ++i) { 
            uint16_t opcode = machine.memory[machine.PC] << 0x8 | machine.memory[machine.PC + 1];
            machine.PC += 2;
            dispatch(machine, opcode);
        }

        Keypad(machine);
        BeginDrawing();
        draw(machine);
        ClearBackground(LIGHTGRAY);
        EndDrawing();

        if (machine.DT > 0) --machine.DT;
        if (machine.ST > 0) --machine.ST;
    }
    return 0;
}
