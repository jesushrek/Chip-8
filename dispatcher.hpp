#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "chip8.hpp"
#include "chip8Instructions.hpp"

void execute(Chip8* subject, uint16_t opcode)
{ 
    uint8_t category = (opcode >> 12) & 0xf;

    switch (category)
    { 
        case 0x0:
            if (opcode == 0x00E0) 
                Cls(subject);
            else if (opcode == 0x00EE) 
                Ret(subject);
            break;

        case 0x1:
            Jp_Addr(subject, opcode);
            break;

        case 0x2:
            Call_Addr(subject, opcode);
            break;

        case 0x3:
            Se_Vx_Byte(subject, opcode);
            break;

        case 0x4:
            SNE_Vx_Byte(subject, opcode);
            break;

        case 0x5:
            SE_Vx_Vy(subject, opcode);
            break;

        case 0x6:
            LD_Vx_byte(subject, opcode);
            break;

        case 0x7:
            Add_Vx_byte(subject, opcode);
            break;

        case 0x8:
            switch (opcode & 0x000f)
            { 
                case 0x0:
                    LD_Vx_Vy(subject, opcode);
                    break;

                case 0x1:
                    OR_Vx_Vy(subject, opcode);
                    break;

                case 0x2:
                    AND_Vx_Vy(subject, opcode);
                    break;

                case 0x3:
                    XOR_Vx_Vy(subject, opcode);
                    break;

                case 0x4:
                    ADD_Vx_Vy(subject, opcode);
                    break;

                case 0x5:
                    SUB_Vx_Vy(subject, opcode);
                    break;

                case 0x6:
                    SHR_VX_Brac_Comma_Vy_Close(subject, opcode);
                    break;

                case 0x7:
                    SUBN_Vx_Vy(subject, opcode);
                    break;

                case 0xE:
                    SHL_VX_Brac_Comma_Vy_Close(subject, opcode);
                    break;
            }
            break;

        case 0x9:
            SNE_Vx_Vy(subject, opcode);
            break;

        case 0xA:
            LD_I_Addr(subject, opcode);
            break;

        case 0xB:
            JP_V0_Addr(subject, opcode);
            break;

        case 0xC:
            RND_Vx_byte(subject, opcode);
            break;

        case 0xD:
            DRW_Vx_Vy_nibble(subject, opcode);
            break;

        case 0xE:
            switch (opcode & 0x00ff)
            { 
                case 0xA1:
                    SKNP_Vx(subject, opcode);
                    break;
                case 0x9E:
                    SKP_Vx(subject, opcode);
                    break;
            }
            break;

        case 0xF:
            switch (opcode & 0x00ff)
            { 
                case 0x07:
                    LD_Vx_Dt(subject, opcode);
                    break;

                case 0x0A:
                    LD_VX_K(subject, opcode);
                    break;

                case 0x15:
                    LD_DT_Vx(subject, opcode);
                    break;

                case 0x18:
                    LD_ST_Vx(subject, opcode);
                    break;

                case 0x1E:
                    ADD_I_Vx(subject, opcode);
                    break;

                case 0x29:
                    LD_F_Vx(subject, opcode);
                    break;

                case 0x33:
                    LD_B_Vx(subject, opcode);
                    break;

                case 0x55:
                    LD_Brac_I_Close_Vx(subject, opcode);
                    break;

                case 0x65:
                    LD_Vx_brac_I_Close(subject, opcode);
                    break;
            }
    }
}

#endif
