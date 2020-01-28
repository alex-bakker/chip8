#include <chip8.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

//Start storing 512 since that space was originally for this interpreter
const int Chip8::MEM_START = 0x200;

Chip8::Chip8() {
    init();
}

void Chip8::init() {
    for(int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    for(int i = 0; i < 16; i++){
        keyboard[i] = 0;
        reg[i] = 0;
    }
    for(int i = 0; i < 64 * 32; i++){
        display[i] = 0;
    }
    while(stack.empty()){
        stack.pop();
    }
    delayTimer = 0;
    soundTimer = 0;

    PC = MEM_START;
    opcode = 0;
    I = 0;

    //TODO: Load in the fontset
}

//Load the specified rom into our 'memory'
//Todo: Add some length checks and use a relative path
void Chip8::loadRom(std::string rom) {
    std::ifstream romFile(rom, std::ios::binary | std::ios::ate);
    if(romFile.is_open()){
        std::streampos size = romFile.tellg();
        char *buffer = new char[size];

        romFile.seekg(0, std::ios::beg);
        romFile.read(buffer, size);
        romFile.close();

        for(int i = 0; i < size; i++)
            memory[MEM_START + i] = buffer[i];

        delete[] buffer;
    }
}

void Chip8::cycle() {
    //Load in the current opcode
    opcode = memory[PC] << 8 | memory[PC + 1];
    drawFlag = false;

    //Used for convenience
    unsigned short X = (opcode & 0x0F00) >> 8;
    unsigned short Y = (opcode & 0x00X0) >> 4;

    //Take action based on the specific opcode.
    switch(opcode & 0xF000) {
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:
                    for(int i = 0; i < 64*32; i++){
                        display[i] = 0;
                    }
                    PC += 2;
                    drawFlag = true;
                    break;

                case 0x000E:
                    PC = stack.top() + 2;
                    stack.pop();
                    break;
                default:
                    //Invalid opcode
                    break; 
             }
             break;

        case 0x1000:
            PC =  opcode & 0x0FFF;
            break;
        case 0x2000:
            stack.push(PC);
            PC = opcode & 0x0FFF;
            break;
        case 0x3000:
            if(reg[X] == opcode & 0x00FF)
                PC += 2;
            PC += 2;
            break;
        case 0x4000:
            if(reg[X] != opcode & 0x00FF)
                PC += 2;
            PC += 2;
            break; 
        case 0x5000:
            if(reg[X] == reg[Y])
                PC += 2;
            PC += 2;
            break;
        case 0x6000:
            reg[X] = opcode & 0x00FF;
            PC += 2;
            break;
        case 0x7000:
            reg[X] += opcode & 0x00FF;
            PC += 2;
            break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    reg[X] = reg[Y];
                    PC += 2;
                    break;
                case 0x0001:
                    reg[X] |= reg[Y];
                    PC += 2;
                    break;
                case 0x0002:
                    reg[X] &= reg[Y];
                    PC += 2;
                    break;
                case 0x0003:
                    reg[X] ^= reg[Y];
                    PC += 2;
                    break;
                case 0x0004:
                    if(reg[X] > (0xFF - reg[Y]))
                        reg[0xF] = 1;
                    else
                        reg[0xF] = 0;
                    reg[X] += reg[Y];
                    PC += 2;
                    break;
                case 0x0005:
                    if(reg[X] > (reg[Y]))
                        reg[0xF] = 0;
                    else
                        reg[0xF] = 1;
                    reg[X] -= reg[Y];
                    PC += 2;
                    break;
                case 0x0006:
                    reg[0xF] = reg[X] & 0x1;
                    reg[X] >>= 0x1;
                    PC += 2;
                    break;
                case 0x0007:
                    if(reg[X] > (reg[Y]))
                        reg[0xF] = 1;
                    else
                        reg[0xF] = 0;

                    reg[X] = reg[Y] - reg[X];
                    PC += 2;
                    break;
                case 0x000E:
                    reg[0xF] = reg[X] >> 7;
                    reg[X] <<= 0x1;
                    PC += 2;
                    break;
                default:
                    //Invalid opcode
            }
            break;
            
    }
}























