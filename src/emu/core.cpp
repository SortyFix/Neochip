#include "core.h"
#include <cstdint>
#include <cstdlib>
#include <fstream>

#define FONTSET_START 0x050

uint8_t fontset[80] = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,
    // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

NC8_Core::NC8_Core(uint8_t *rom, size_t rom_size, NC8_Display* dsp)
{
    loadFonts();
    loadROM(rom, rom_size);
    display = dsp;
}

void NC8_Core::loadFonts() {
    for(int i = 0; i < FONTSET_SIZE; i++)
    {
        memory[FONTSET_START + i] = fontset[i];
    }
}

void NC8_Core::loadROM(uint8_t *rom, size_t rom_size) {
    for(int i = 0; i < rom_size; i++)
    {
        rom_buffer[i] = rom[i];
    }
}

void NC8_Core::tick()
{
    
}

void NC8_Core::setVF(bool val)
{
    g_reg[0xF] = val;
}

/**
    Opcode 00E0 - CLS
    Clears the display.
*/
void NC8_Core::OP_00E0()
{
    display->clear();
}

/*
    Opcode 00EE - RET
    Return from a subroutine
*/
void NC8_Core::OP_00EE()
{
    sp--;
    pc = stack[sp];
}

/*
    Opcode 1nnn - JMP
    Jump to location by setting the program counter PC to nnn
*/
void NC8_Core::OP_1nnn()
{
    uint16_t mem_addr = opcode & 0x0FFF;
    pc = mem_addr;
}

/*
    Opcode 2nnn - CALL
    Set program counter PC to nnn, but save the current PC on the top of the stack
*/
void NC8_Core::OP_2nnn()
{
    uint16_t mem_addr = opcode & 0x0FFF;
    stack[sp] = pc;
    sp++;
    pc = mem_addr;
}

/*
    Opcode 3xkk - SE Vx, Byte
    Compare register at location x with byte kk. If equal, increment program counter by 2.
*/
void NC8_Core::OP_3xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    
    if(g_reg[x] == kk)
    {
        pc += 2;
    }
}

/*
    Opcode 4xkk - SNE Vx, Byte
    Inversion of 3xkk; If NOT equal, increment program counter by 2.
*/
void NC8_Core::OP_4xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    if(g_reg[x] != kk)
    {
        pc += 2;
    }
}

void NC8_Core::OP_5xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if(x == y)
    {
        pc += 2;
    }
}

void NC8_Core::OP_6xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    g_reg[x] = kk;
}

void NC8_Core::OP_7xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    uint8_t new_val = g_reg[x] + kk;
    g_reg[x] = new_val;
}

void NC8_Core::OP_8xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    g_reg[x] = g_reg[y];
}

void NC8_Core::OP_8xy1()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    g_reg[x] |= g_reg[y];
}

void NC8_Core::OP_8xy2()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    g_reg[x] &= g_reg[y];
}

void NC8_Core::OP_8xy3()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    g_reg[x] ^= g_reg[y];
}

void NC8_Core::OP_8xy4()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t result = (g_reg[x] + g_reg[y]);
    g_reg[x] = result & 0x00FF;

    if(result > 255)
    {
        g_reg[0xF] = 1;
        return;
    }

    g_reg[0xF] = 0;
}

void NC8_Core::OP_8xy5()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    g_reg[x] = g_reg[x] - g_reg[y];

    if(g_reg[x] > g_reg[y])
    {
        g_reg[0xF] = 1;
        return;
    }

    g_reg[0xF] = 0;
}

void NC8_Core::OP_8xy6()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    if((g_reg[x] & 0x0001) == 1) g_reg[0xF] = 1; else g_reg[0xF] = 0;
    g_reg[x] /= 2;
}

void NC8_Core::OP_8xy7()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if(g_reg[y] > g_reg[x]) g_reg[0xF] = 1; else g_reg[0xF] = 0;
    g_reg[x] = g_reg[y] - g_reg[x];
}

void NC8_Core::OP_8xyE()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    if((g_reg[x] & 0x8000) == 1) g_reg[0xF] = 1; else g_reg[0xF] = 0;
    g_reg[x] *= 2;
}

void NC8_Core::OP_9xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if(g_reg[x] != g_reg[y]) pc += 2;
}

void NC8_Core::OP_Annn()
{
    uint16_t nnn = opcode & 0x0FFF;
    i_reg = nnn;
}

void NC8_Core::OP_Bnnn()
{
    uint16_t nnn = opcode & 0x0FFF;
    pc = nnn + g_reg[0x0];
}

void NC8_Core::OP_Cxkk()
{
    uint8_t random_num = rand() % 256;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    g_reg[x] = random_num & kk;
}

void NC8_Core::OP_Dxyn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;

    uint8_t n_bytes[n];

    for(int index = 0; index < n; index++)
    {
        n_bytes[index] = memory[i_reg + index];
    } 

    display->drawSprite(g_reg[x], g_reg[y], n_bytes, n, 
        [this]() -> void { g_reg[0xF] = 1; });
}

// TODO BEGIN

// TODO END

void NC8_Core::OP_Fx1E()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    i_reg = i_reg + g_reg[x];
}

void NC8_Core::OP_Fx29()
{                                         
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t fontset_index = x * 5;
    i_reg = FONTSET_START + fontset_index;
}

void NC8_Core::OP_Fx33()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    memory[i_reg] = g_reg[x] / 100 % 10;
    memory[i_reg+1] = g_reg[x] / 10 % 10;
    memory[i_reg+2] = g_reg[x] % 10;
}

void NC8_Core::OP_Fx55()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int index = 0; index <= x; index++)
    {
        memory[i_reg + index] = g_reg[index];
    }
}

void NC8_Core::OP_Fx65()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int index = 0; index <= x; index++)
    {
        g_reg[index] = memory[i_reg + index];
    }
}
