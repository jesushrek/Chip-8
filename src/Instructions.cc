#include "Chip8.hh"

#include <cstring>
#include <raylib.h>


uint16_t extractNNN(uint16_t opcode) { return (opcode & 0x0FFF); }
uint8_t extractX(uint16_t opcode) { return ((opcode >> 0x08) & 0x000F); }
uint8_t extractY(uint16_t opcode) { return ((opcode >> 0x04) & 0x000F); }
uint8_t extractKK(uint16_t opcode) { return (opcode & 0x00FF); }
uint16_t extractNibble(uint16_t opcode) { return (opcode & 0x000F); }

// 00E0
void CLS(Chip8 &subject) { std::memset(subject.screen, 0, 64 * 32 * sizeof *subject.screen); }

// 00EE
void RET(Chip8 &subject) 
{ 
    if (subject.SC == 0) return;
    subject.PC = subject.stack[--subject.SC];
}

// 1nnn
void JP_addr(Chip8 &subject, uint16_t opcode) { subject.PC = extractNNN(opcode); }

// 2nnn
void CALL_addr(Chip8 &subject, uint16_t opcode) 
{ 
    if (subject.SC >= 16) return;
    subject.stack[subject.SC++] = subject.PC;
    JP_addr(subject, opcode);
}

// 3xkk
void SE_Vx_byte(Chip8 &subject, uint16_t opcode) 
{ 
    if (subject.V[extractX(opcode)] == extractKK(opcode)) subject.PC += 2;
}

// 4xkk
void SNE_VX_byte(Chip8 &subject, uint16_t opcode)
{ 
    if (subject.V[extractX(opcode)] != extractKK(opcode)) subject.PC += 2;
}

// 5xy0
void SE_VX_VY(Chip8 &subject, uint16_t opcode)
{ 
    if (subject.V[extractX(opcode)] == subject.V[extractY(opcode)]) subject.PC += 2;
}

// 6xkk
void LD_VX_BYTE(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] = extractKK(opcode); }

// 7xkk
void ADD_VX_BYTE(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] +=  extractKK(opcode); }

// 8xy0
void LD_VX_VY(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] = subject.V[extractY(opcode)]; }

// 8xy1
void OR_VX_VY(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] |= subject.V[extractY(opcode)]; }

// 8xy2
void AND_VX_VY(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] &= subject.V[extractY(opcode)]; }

// 8xy3
void XOR_VX_VY(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] ^= subject.V[extractY(opcode)]; }

// 8xy4
void ADD_VX_VY(Chip8 &subject, uint16_t opcode) 
{ 
    uint16_t result = (subject.V[extractX(opcode)] + subject.V[extractY(opcode)]);
    subject.V[0x0F] = (result > 0x0FF); // 0x0FF255
    subject.V[extractX(opcode)] = result & 0x00FF;
}

// 8xy5
void SUB_VX_VY(Chip8 &subject, uint16_t opcode) 
{ 
    subject.V[0x0F] = (subject.V[extractX(opcode)] > subject.V[extractY(opcode)]);
    subject.V[extractX(opcode)] -= subject.V[extractY(opcode)]; 
}

// 8xy6
void SHR_VX_VY(Chip8 &subject, uint16_t opcode)
{ 
    subject.V[0x0F] = subject.V[extractX(opcode)] & 0x1;
    subject.V[extractX(opcode)] /= 2;
}

// 8xy7
void SUBN_VX_VY(Chip8 &subject, uint16_t opcode)
{ 
    subject.V[0x0F] = (subject.V[extractY(opcode)] > subject.V[extractX(opcode)]);
    subject.V[extractX(opcode)] = (subject.V[extractY(opcode)] - subject.V[extractX(opcode)]);
}

// 8xyE
void SHL_VX_VY(Chip8 &subject, uint16_t opcode)
{ 
    subject.V[0x0F] = (subject.V[extractX(opcode)] & 0x80)? 1 : 0; 
    subject.V[extractX(opcode)] *= 2;;
}

// 9xy0
void SNE_VX_VY(Chip8 &subject, uint16_t opcode)
{ 
    if (subject.V[extractX(opcode)] != subject.V[extractY(opcode)]) subject.PC += 2;
}

// Annn
void LD_I_ADDR(Chip8 &subject, uint16_t opcode) { subject.I = extractNNN(opcode); }

// Bnnn
void JP_V0_ADDR(Chip8 &subject, uint16_t opcode) { subject.PC = extractNNN(opcode) + subject.V[0x0]; }

// Cxkk
void RND_VX_BYTE(Chip8 &subject, uint16_t opcode) 
{ 
    uint8_t random { static_cast<uint8_t>(GetRandomValue(0, 255)) };
    subject.V[extractX(opcode)] = random & extractKK(opcode);
}

// Dxyn
void DRW_VX_VY_NIBBLE(Chip8 &subject, uint16_t opcode)
{ 
    int starting_x { subject.V[extractX(opcode)] % 64 };
    int starting_y { subject.V[extractY(opcode)] % 32 };
    int sprite_height { extractNibble(opcode) };

    subject.V[0x0F] = 0;
    for (int row = 0; row < sprite_height; ++row) { 
        uint8_t sprite_memory { subject.memory[subject.I + row] };
        int height { (starting_y + row) % 32 };

        for (int col = 0; col < 8; ++col) { 
            if (sprite_memory & (0x80 >> col)) { 
                int width  { (starting_x + col) % 64 };

                int raster_pos { height * 64 + width };
                if (subject.screen[raster_pos]) subject.V[0x0F] = 1;
                subject.screen[raster_pos] ^= 1;
            }
        }
    }
}

// Ex9E
void SKP_VX(Chip8 &subject, uint16_t opcode) { if (subject.keyPad[subject.V[extractX(opcode)]]) subject.PC += 2; }

// ExA1
void SKNP_VX(Chip8 &subject, uint16_t opcode) { if (!subject.keyPad[subject.V[extractX(opcode)]]) subject.PC += 2; } 

// Fx07
void LD_VX_DT(Chip8 &subject, uint16_t opcode) { subject.V[extractX(opcode)] = subject.DT; }

// Fx0A
void LD_VX_K(Chip8 &subject, uint16_t opcode) 
{ 
    bool keyPressed {};
    int x { extractX(opcode) };

    for (int i = 0; i < 16; ++i) { 
        if (subject.keyPad[i]) { 
            subject.V[x] = static_cast<uint8_t>(i);
            keyPressed = true;
            break;
        }
    }

    if (!keyPressed) subject.PC -= 2;
}

// Fx15
void LD_DT_VX(Chip8 &subject, uint16_t opcode) { subject.DT = subject.V[extractX(opcode)]; }

// Fx18
void LD_ST_VX(Chip8 &subject, uint16_t opcode) { subject.ST = subject.V[extractX(opcode)]; }

// Fx1E
void ADD_I_VX(Chip8 &subject, uint16_t opcode) { subject.I = subject.I + subject.V[extractX(opcode)]; }

// Fx29
void LD_F_VX(Chip8 &subject, uint16_t opcode) { subject.I = (subject.V[extractX(opcode)] & 0x000f) * 0x05; }

// Fx33
void LD_B_VX(Chip8 &subject, uint16_t opcode) { 
    subject.memory[subject.I] = subject.V[extractX(opcode)] / 100;
    subject.memory[subject.I + 1] = (subject.V[extractX(opcode)] / 10) % 10;
    subject.memory[subject.I + 2] = subject.V[extractX(opcode)] % 10;
}

// Fx55
void LD_I_VX(Chip8 &subject, uint16_t opcode) 
{ 
    int x { extractX(opcode) };
    for (int i = 0; i <= x; ++i) subject.memory[subject.I + i] = subject.V[i];
}

// Fx65
void LD_VX_I(Chip8 &subject, uint16_t opcode) 
{ 
    int x { extractX(opcode) };
    for (int i = 0; i <= x; ++i) subject.V[i] = subject.memory[subject.I + i];
}

void dispatch(Chip8 &subject, uint16_t opcode) 
{ 
    uint8_t current_instruction = (opcode >> 12) & 0x0f;

    switch(current_instruction) { 
        case 0x0:
            if (opcode == 0x00E0) CLS(subject); 
            else if (opcode == 0x00EE) RET(subject);
            break;

        case 0x1:
            JP_addr(subject, opcode);
            break;

        case 0x2:
            CALL_addr(subject, opcode);
            break;

        case 0x3:
            SE_Vx_byte(subject, opcode);
            break;

        case 0x4:
            SNE_VX_byte(subject, opcode);
            break;

        case 0x5:
            SE_VX_VY(subject, opcode);
            break;

        case 0x6:
            LD_VX_BYTE(subject, opcode);
            break;

        case 0x7:
            ADD_VX_BYTE(subject, opcode);
            break;

        case 0x8:
            switch(opcode & 0x000F) { 
                case 0x0:
                    LD_VX_VY(subject, opcode);
                    break;
                case 0x1:
                    OR_VX_VY(subject, opcode);
                    break;
                case 0x2:
                    AND_VX_VY(subject, opcode);
                    break;
                case 0x3:
                    XOR_VX_VY(subject, opcode);
                    break;
                case 0x4:
                    ADD_VX_VY(subject, opcode);
                    break;
                case 0x5:
                    SUB_VX_VY(subject, opcode);
                    break;
                case 0x6:
                    SHR_VX_VY(subject, opcode);
                    break;
                case 0x7:
                    SUBN_VX_VY(subject, opcode);
                    break;
                case 0xE:
                    SHL_VX_VY(subject, opcode);
                    break;
            }
            break;

        case 0x9:
            SNE_VX_VY(subject, opcode);
            break;

        case 0xA:
            LD_I_ADDR(subject, opcode);
            break;

        case 0xB:
            JP_V0_ADDR(subject, opcode);
            break;

        case 0xC:
            RND_VX_BYTE(subject, opcode);
            break;

        case 0xD:
            DRW_VX_VY_NIBBLE(subject, opcode);
            break;

        case 0xE:
            switch(opcode & 0x00FF) {
                case 0x9E:
                    SKP_VX(subject, opcode);
                    break;

                case 0xA1:
                    SKNP_VX(subject, opcode);
                    break;
            }

            break;

        case 0xF:
            switch(opcode & 0x00FF) {
                case 0x07:
                    LD_VX_DT(subject, opcode);
                    break;
                case 0x0A:
                    LD_VX_K(subject, opcode);
                    break;
                case 0x15:
                    LD_DT_VX(subject, opcode);
                    break;
                case 0x18:
                    LD_ST_VX(subject, opcode);
                    break;
                case 0x1E:
                    ADD_I_VX(subject, opcode);
                    break;
                case 0x29:
                    LD_F_VX(subject, opcode);
                    break;
                case 0x33:
                    LD_B_VX(subject, opcode);
                    break;
                case 0x55:
                    LD_I_VX(subject, opcode);
                    break;
                case 0x65:
                    LD_VX_I(subject, opcode);
            }

            break;
    }
}
