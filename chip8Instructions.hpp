#ifndef CHIP8INSTRUCTIONS_HPP
#define CHIP8INSTRUCTIONS_HPP

#include "chip8.hpp"
#include <raylib.h>

#include <cstdint>
#include <cstdlib>

void Cls(Chip8* subject) // 00E0
{ 
    for (int i = 0; i < 64 * 32; ++i) subject->screen[i] = 0;
}

void Ret(Chip8* subject) // 00EE
{ 
    if (subject->SP == 0) return;
    subject->PC = subject->stack[--subject->SP];
}

void Jp_Addr(Chip8* subject, uint16_t opcode) // 1NNN
{ 
    uint16_t nnn = opcode & 0x0fff;
    subject->PC = nnn;
}

void Call_Addr(Chip8* subject, uint16_t opcode) // 2NNN
{ 
    if (subject->SP >= 16) return;

    subject->stack[subject->SP++] = subject->PC;
    Jp_Addr(subject, opcode);
}

void Se_Vx_Byte(Chip8* subject, uint16_t opcode) //3xkk
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t kk = (opcode) & 0x00ff;

    if (subject->V[x] == kk) subject->PC += 2;
}

void SNE_Vx_Byte(Chip8* subject, uint16_t opcode) //4xkk
{ 
    /* example
     * 1111 1111 1111 1111
     * 0000 0000 1111 1111
     * 0000 0000 0000 1111
     */

    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t kk = (opcode) & 0x00ff;

    if (subject->V[x] != kk) subject->PC += 2;
}

void SE_Vx_Vy(Chip8* subject, uint16_t opcode) // 5xy0
{

    /* example
     * 1111 1111 1111 1111
     * 0000 1111 1111 1111
     * 0000 0000 0000 1111
     */

    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    if (subject->V[x] == subject->V[y]) subject->PC += 2;
}

void LD_Vx_byte(Chip8* subject, uint16_t opcode) // 6xkk
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t kk = (opcode) & 0x0ff;

    subject->V[x] = kk;
}

void Add_Vx_byte(Chip8* subject, uint16_t opcode) // 7xkk
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t kk = (opcode) & 0x0ff;

    subject->V[x] += kk;
}

void LD_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy0
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[x] = subject->V[y];
}

void OR_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy1
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[x] = subject->V[x] | subject->V[y];
}

void AND_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy2
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[x] = subject->V[x] & subject->V[y];
}

void XOR_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy3
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[x] = subject->V[x] ^ subject->V[y];
}

void ADD_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy4
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    uint16_t result = subject->V[x] + subject->V[y];

    subject->V[0x0f] = (result > 255);
    subject->V[x] = (uint8_t)(result);
}

void SUB_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy5
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[0x0f] = (subject->V[x] > subject->V[y]);

    subject->V[x] -= subject->V[y];
}

void SHR_VX_Brac_Comma_Vy_Close(Chip8* subject, uint16_t opcode) // 8xy6
{
    uint8_t x = (opcode >> 8) & 0x000f;

    subject->V[0x0f] = (subject->V[x] & 0x01);
    subject->V[x] /= 2;
}

void SUBN_Vx_Vy(Chip8* subject, uint16_t opcode) // 8xy7
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    subject->V[0x0f] = (subject->V[y] > subject->V[x]); 
    subject->V[x] = subject->V[y] - subject->V[x];
}

void SHL_VX_Brac_Comma_Vy_Close(Chip8* subject, uint16_t opcode) // 8xyE
{
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->V[0x0f] = (subject->V[x] & 0x80);

    subject->V[x] *= 2;
}

void SNE_Vx_Vy(Chip8* subject, uint16_t opcode) // 9xy0
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    if (subject->V[x] != subject->V[y]) subject->PC += 2;
}

void LD_I_Addr(Chip8* subject, uint16_t opcode) // Annn
{
    uint16_t nnn = opcode & 0x0fff;
    subject->I = nnn;
}

void JP_V0_Addr(Chip8* subject, uint16_t opcode) // Bnnn
{
    uint16_t nnn = opcode & 0x0fff;
    subject->PC = nnn + subject->V[0];
}

void RND_Vx_byte(Chip8* subject, uint16_t opcode) // Cxkk
{
    uint8_t x = (opcode >> 8) & 0x000f;

    uint8_t randomValue { uint8_t(GetRandomValue(0, 255)) };
    uint8_t kk = opcode & 0x00ff;

    subject->V[x] = kk & randomValue;
}

void DRW_Vx_Vy_nibble(Chip8* subject, uint16_t opcode) // Dxyn
{
    uint8_t x = (opcode >> 8) & 0x000f;
    uint8_t y = (opcode >> 4) & 0x000f;

    uint8_t spriteHeight = opcode & 0x000f;

    uint8_t starting_x = subject->V[x];
    uint8_t starting_y = subject->V[y];

    subject->V[0x0f] = 0;

    for (int row = 0; row < spriteHeight; ++row) 
    { 
        uint8_t sprite_memory = subject->Memory[subject->I + row];
        for (int col = 0; col < 8; ++col) 
        { 
            if (sprite_memory & (0x80 >> col))
            { 
                int height = (starting_y + row) % 32; 
                int width  = (starting_x + col) % 64;

                if (subject->screen[height * 64 + width])
                {
                    subject->V[0x0f] = 1;
                }

                subject->screen[height * 64 + width] ^= 1;
            }
        }
    }

}

void SKP_Vx(Chip8* subject, uint16_t opcode) // Ex9E
{
    uint8_t x = (opcode >> 8) & 0x000f;
    if (subject->keyboard[subject->V[x]]) subject->PC += 2;
}

void SKNP_Vx(Chip8* subject, uint16_t opcode) // ExA1
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    if (!subject->keyboard[subject->V[x]]) subject->PC += 2;
}

void LD_Vx_Dt(Chip8* subject, uint16_t opcode) // Fx07
{ 
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->V[x] = subject->DT;
}

void LD_VX_K(Chip8* subject, uint16_t opcode) //Fx0A
{
    uint8_t x = (opcode >> 8) & 0x000f;

    bool isKeyPressed { false };
    for (uint8_t i = 0; i < 16; ++i)
    { 
        if (subject->keyboard[i])
        { 
            subject->V[x] = i;
            isKeyPressed = true;
            break;
        }
    }

    if (!isKeyPressed)
        subject->PC -= 2;
}

void LD_DT_Vx(Chip8* subject, uint16_t opcode) // Fx015
{
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->DT = subject->V[x];
}

void LD_ST_Vx(Chip8* subject, uint16_t opcode) // Fx18
{
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->ST = subject->V[x];
}

void ADD_I_Vx(Chip8* subject, uint16_t opcode) // Fx1E
{
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->I += subject->V[x];
}

void LD_F_Vx(Chip8* subject, uint16_t opcode) // fx29
{
    uint8_t x = (opcode >> 8) & 0x000f;
    subject->I = ((subject->V[x] & 0x000f) * 0x5);
}

void LD_B_Vx(Chip8* subject, uint16_t opcode) // fx33
{
    uint8_t x = (opcode >> 8) & 0x000f;

    subject->Memory[subject->I] = subject->V[x] / 100; 
    subject->Memory[subject->I + 1] = (subject->V[x] / 10) % 10;
    subject->Memory[subject->I + 2] = (subject->V[x]) % 10;
}

void LD_Brac_I_Close_Vx(Chip8* subject, uint16_t opcode) // fx55
{
    uint8_t x = (opcode >> 8) & 0x000f;

    for (uint8_t i = 0; i <= x; ++i) subject->Memory[subject->I + i] = subject->V[i];
}

void LD_Vx_brac_I_Close(Chip8* subject, uint16_t opcode) // fx65
{
    uint8_t x = (opcode >> 8) & 0x000f;

    for (uint8_t i = 0; i <= x; ++i) subject->V[i] = subject->Memory[subject->I + i];
}

#endif
