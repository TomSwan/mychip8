// interpreter.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include "interpreter.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;  

unsigned char chip8_fontset[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Interpreter::Interpreter() {
}

Interpreter::~Interpreter() {
}

void Interpreter::init()
{
    pc       = 0x200;
    opcode   = 0;
    I        = 0;
    sp       = 0;

    for(int i = 0; i < 2048; i++)
        gfx[i] = 0;
    for(int i = 0; i < 16; i++)
        stack[i] = 0;
    for(int i = 0; i < 16; i++)
        key[i] = V[i] = 0;
    for(int i = 0; i < 4096; i++)
        memory[i] = 0;
    for(int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];        

    delay_timer = 0;
    sound_timer = 0;

    srand (time(NULL));
}

void Interpreter::emulateCycle()
{
    unsigned char flag;
    
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch(opcode & 0xF000)
    {        
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clear screen
                    for(int i = 0; i < 2048; i++)
                        gfx[i] = 0x0;
                    pc += 2;
                break;

                case 0x000E: // 0x00EE: Return from subroutine
                    --sp;
                    pc = stack[sp];
                    pc += 2;        
                break;

                default:
                    printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
                    exit(1);
            }

        break;

        case 0x1000: // 0x1NNN: Jump to 0NNN
            pc = opcode & 0x0FFF;
        break;

        case 0x2000: // 0x2NNN: Call subroutine at 0NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
        break;
        
        case 0x3000: // 0x3XNN: Skip next if VX == NN
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;
        
        case 0x4000: // 0x4XNN: Skip next if VX != NN
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;
        
        case 0x5000: // 0x5XY0: Skip next if VX == VY
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break;
        
        case 0x6000: // 0x6XNN: Assign NN to VX
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
        break;
            
        case 0x7000: // 0x7XNN: Add NN to VX
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
        break;
        
        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x8XY0: Assign VY to VX
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0001: // 0x8XY1: Set VX to VX "OR" VY
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0002: // 0x8XY2: Set VX to VX "AND" VY
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0003: // 0x8XY3: Set VX to VX "XOR" VY
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0004: // 0x8XY4: Add VY to VX; VF: carry(1) no-carry(0)
                    flag = 0;
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) 
                        flag = 1;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    V[0xF] = flag;
                    pc += 2;                    
                break;

                case 0x0005: // 0x8XY5: VX - VY, VF: borrow(1) no-borrow(0)
                    flag = 1;
                    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) 
                        flag = 0;
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    V[0xF] = flag;
                    pc += 2;
                break;

                case 0x0006: // 0x8XY6: Shift VY right by one into VX; VF==LSB before
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    flag = V[(opcode & 0x00F0) >> 4] & 0x0001;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    V[0xF] = flag;
                    pc += 2;
                break;

                case 0x0007: // 0x8XY7: Set VX to VY minus VX; VF(0)==borrow, (1)==no borrow
                    flag = 1;
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                        flag = 0;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    V[0xF] = flag;
                    pc += 2;
                break;

                case 0x000E: // 0x8XYE: Shift VY left by one into VX; VF==MSB before
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    flag = V[(opcode & 0x00F0) >> 4] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    V[0xF] = flag;
                    pc += 2;
                break;

                default:
                    printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
                    exit(1);
            }
        break;
        
        case 0x9000: // 0x9XY0: Skip next if VX != VY
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break;

        case 0xA000: // 0xANNN: Set I to 0NNN
            I = opcode & 0x0FFF;
            pc += 2;
        break;
        
        case 0xB000: // 0xBNNN: Jump to 0NNN + V0
            pc = (opcode & 0x0FFF) + V[0];
        break;
        
        case 0xC000: // 0xCXNN: Set VX to random number modulo NN
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            pc += 2;
        break;
 
        case 0xD000: // 0xDXYN: Show N-byte pattern from M(I) at (VX, VY) via XOR. 
                     // VF==1 if any display and pattern bits == 1; I unchanged. 
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[I + yline];
                for(int xrow = 0; xrow < 8; xrow++)
                {
                    if((pixel & (0x80 >> xrow)) != 0)
                    {
                        int xt = (x + xrow ) % 64;
                        int yt = (y + yline) % 32;
                        int ti = xt + (yt * 64);
                        if(gfx[ti] == 1) {
                            V[0xF] = 1;                                    
                        }
                        gfx[ti] ^= 1;
                    }
                }
            }
            pc += 2;
        }
        break;
            
        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E: // 0xEX9E: Skip next if key VX pressed
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                        pc += 4;
                    else
                        pc += 2;
                break;
                
                case 0x00A1: // 0xEXA1: Skip next if key VX not pressed
                    if(key[V[(opcode & 0x0F00) >> 8]] == 0)
                        pc += 4;
                    else
                        pc += 2;
                break;

                default:
                    printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
                    exit(1);
            }
        break;
        
        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: // 0xFX07: Set VX to delay timer value
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break;

                case 0x000A: // 0xFX0A: Wait for and store keypress in VX
                {
                    bool KP = false;
                    for(int i = 0; i < 16; i++) {
                        if(key[i] != 0) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            KP = true;
                        }
                    }
                    if(KP) pc += 2;                    
                }
                break;
                
                case 0x0015: // 0xFX15: Set delay timer to VX
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x0018: // 0xFX18: Set sound timer to VX
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x001E: // 0xFX1E: Add VX to I
                    I += V[(opcode & 0x0F00) >> 8] % 0xFFF;
                    pc += 2;
                break;

                case 0x0029: // 0xFX29: Set I to sprite for character in VX
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                break;

                case 0x0033: // 0xFX33: Store BCD of VX to M(I, I+1, I+2)
                    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;                    
                    pc += 2;
                break;

                case 0x0055: // 0xFX55: Store V0 through VX to M[I, ..., I+VN]
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                        memory[I++] = V[i];
                    pc += 2;
                break;

                case 0x0065: // 0xFX65: Load V0 through VX from M[I, ..., I+VN]
                    for (int i = 0; (i <= ((opcode & 0x0F00) >> 8)); i++) {
                        V[i] = memory[I++];
                    }
                    pc += 2;
                break;

                default:
                    printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
                    exit(1);
            }
        break;

        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
                    exit(1);
    }    

}

bool Interpreter::loadApplication(const char * filename)
{
    init();
    printf("Loading: %s\n", filename);
        
    FILE * pFile = fopen(filename, "rb");
    if (pFile == NULL)
    {
        fputs ("File error", stderr); 
        return false;
    }

    fseek(pFile , 0 , SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    printf("Filesize: %d\n", (int)lSize);
    
    char * buffer = (char*)malloc(sizeof(char) * lSize);
    if (buffer == NULL) 
    {
        fputs ("Memory error", stderr); 
        return false;
    }

    int result = fread (buffer, 1, lSize, pFile);
    if (result != lSize) 
    {
        fputs("Reading error",stderr); 
        return false;
    }

    if((4096-512) > lSize)
    {
        for(int i = 0; i < lSize; i++)
            memory[i + 512] = buffer[i];
    }
    else
        printf("Error: ROM too big for memory");
    
    fclose(pFile);
    free(buffer);

    return true;
}

unsigned short Interpreter::getpc() const {
    return pc;
}
    
unsigned short Interpreter::getopcode() const  {
    return opcode;
}
    
unsigned short Interpreter::getI() const  {
    return I;
}
    
unsigned short Interpreter::getsp() const  {
    return sp;
}
    
unsigned char  Interpreter::getV(int i) const  {
    return V[i];
}
    
unsigned short Interpreter::getstack(int i) const  {
    return stack[i];
}
    
unsigned char  Interpreter::getmemory(int i) const  {
    return memory[i];
}

// Original file header 2020-05-24 ts
///////////////////////////////////////////////////////////////////////////////
// Project description
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Name: myChip8
//
// Author: Laurence Muller
// Contact: laurence.muller@gmail.com
//
// License: GNU General Public License (GPL) v2 
// ( http://www.gnu.org/licenses/old-licenses/gpl-2.0.html )
//
// Copyright (C) 2011 Laurence Muller / www.multigesture.net
///////////////////////////////////////////////////////////////////////////////
