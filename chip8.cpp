#include "chip8.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>

unsigned char fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, 
  0x20, 0x60, 0x20, 0x20, 0x70, 
  0xF0, 0x10, 0xF0, 0x80, 0xF0, 
  0xF0, 0x10, 0xF0, 0x10, 0xF0, 
  0x90, 0x90, 0xF0, 0x10, 0x10, 
  0xF0, 0x80, 0xF0, 0x10, 0xF0, 
  0xF0, 0x80, 0xF0, 0x90, 0xF0, 
  0xF0, 0x10, 0x20, 0x40, 0x40, 
  0xF0, 0x90, 0xF0, 0x90, 0xF0, 
  0xF0, 0x90, 0xF0, 0x10, 0xF0, 
  0xF0, 0x90, 0xF0, 0x90, 0x90, 
  0xE0, 0x90, 0xE0, 0x90, 0xE0, 
  0xF0, 0x80, 0x80, 0x80, 0xF0, 
  0xE0, 0x90, 0x90, 0x90, 0xE0, 
  0xF0, 0x80, 0xF0, 0x80, 0xF0, 
  0xF0, 0x80, 0xF0, 0x80, 0x80  
};

//Start storing 512 since that space was originally for this interpreter
const int Chip8::MEM_START = 0x200;

Chip8::Chip8(std::string rom) {
    init();
    loadRom(rom);
}

void Chip8::init() {
    for(int i = 80; i < 4096; i++){
        memory[i] = 0;
    }
    for(int i = 0; i < 16; i++){
        keyboard[i] = 0;
        reg[i] = 0;
    }
    for(int i = 0; i < 2048; i++){
        display[i] = 0;
    }
    while(!stack.empty()){
        stack.pop();
    }

    for(int i = 0; i < 80; i++){
        memory[i] = fontset[i];
    }

    delayTimer = 0;
    soundTimer = 0;

    PC = MEM_START;
    opcode = 0;
    I = 0;
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

//Update the internal key register
void Chip8::updateKey(int index, uint8_t val){
    keyboard[index] = val;
}

void Chip8::cycle() {
    //Load in the current opcode
    opcode = (memory[PC] << 8) | (memory[PC + 1]);
    drawFlag = false;

    //Used for convenience
    unsigned short X = (opcode & 0x0F00) >> 8;
    unsigned short Y = (opcode & 0x00F0) >> 4;

    unsigned short xCord;
    unsigned short yCord;
    unsigned short height;

    bool keyPressed;

    std::cout << "PC = " << PC << " : " << std::hex << opcode << "(" << std::dec << opcode << ")" << std::endl;

    //Take action based on the specific opcode.
    switch(opcode & 0xF000) {
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:
                    for(int i = 0; i < 2048; i++){
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
                    std::cout << "INVALID" << std::endl;
                    break; 
             }
             break;

        case 0x1000:
            PC = opcode & 0x0FFF;
            break;
        case 0x2000:
            stack.push(PC);
            PC = opcode & 0x0FFF;
            break;
        case 0x3000:
            if(reg[X] == (opcode & 0x00FF))
                PC += 2;
            PC += 2;
            break;
        case 0x4000:
            if(reg[X] != (opcode & 0x00FF))
                PC += 2;
            PC += 2;
            break; 
        case 0x5000:
            if(reg[X] == reg[Y])
                PC += 2;
            PC += 2;
            break;
        case 0x6000:
            reg[X] = (opcode & 0x00FF);
            PC += 2;
            break;
        case 0x7000:
            reg[X] += (opcode & 0x00FF);
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
                    if(reg[X] >= (reg[Y]))
                        reg[0xF] = 1;
                    else
                        reg[0xF] = 0;
                    reg[X] -= reg[Y];
                    PC += 2;
                    break;
                case 0x0006:
                    reg[0xF] = reg[X] & 0x1;
                    reg[X] >>= 1;
                    PC += 2;
                    break;
                case 0x0007:
                    if(reg[X] > (reg[Y]))
                        reg[0xF] = 0;
                    else
                        reg[0xF] = 1;

                    reg[X] = reg[Y] - reg[X];
                    PC += 2;
                    break;
                case 0x000E:
                    reg[0xF] = reg[X] >> 7;
                    reg[X] <<= 1;
                    PC += 2;
                    break;
                default:
                    std::cout << "INVALID" << std::endl;
                    break;
            }
            break;
        case 0x9000:
            if(reg[X] != reg[Y])
                PC += 2;
            PC += 2;
            break;
        case 0xA000:
            I = opcode & 0x0FFF;
            PC += 2;
            break;
        case 0xB000:
            PC = reg[0] + (opcode & 0x0FFF);
            break;
        case 0xC000:
            reg[X] = (rand() % 256) & (opcode & 0x00FF);
            PC += 2;
            break;
        case 0xD000:
            xCord = reg[X];
            yCord = reg[Y];
            height = opcode & 0x000F;
            reg[0xF] = 0;
            for(int j = 0; j < height; j++){
                unsigned char row = memory[I + j];
                for(int k = 0; k < 8; k++){
                    if((row & (0x80 >> k)) != 0){
                        if(display[(xCord + k) + (64 * (yCord + j))] == 1)
                            reg[0xF] = 1;
                        display[(xCord + k) + (64 * (yCord + j))] ^= 1;
                    }
                }
            }
            drawFlag = true;
            PC += 2;
            break;
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E:
                    if(keyboard[reg[X]] == 1)
                        PC += 2;
                    PC += 2;
                    break;
                case 0x0001:
                    if(keyboard[reg[X]] != 1)
                        PC += 2;
                    PC += 2;
                    break;
                default:
                    std::cout << "INVALID" << std::endl;
                    break;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:
                    reg[X] = delayTimer;
                    PC += 2;
                    break;
                case 0x000A:
                    keyPressed = false;
                    for(int i = 0; i < 16; i++){
                        bool keyPressed = false;
                        if(keyboard[i] == 1){
                            keyPressed = true;
                            reg[X] = i;
                        }
                    }
                    if(!keyPressed)
                        return;
                    PC += 2;
                    break;
                case 0x0015:
                    delayTimer = reg[X];
                    PC += 2;
                    break;
                case 0x0018:
                    soundTimer = reg[X];
                    PC += 2;
                    break;
                case 0x001E:
                    if(I + reg[X] > 0xFFF)
                        reg[0xF] = 1;
                    else
                        reg[0xF] = 0;
                    I += reg[X];
                    PC += 2;
                    break;
                case 0x0029:
                    I = reg[X] * 5; //Font is stored beginning at index 0!
                    PC += 2;
                    break;
                case 0x0033:
                    memory[I] = reg[X] / 100;
                    memory[I+1] = (reg[X] / 10) % 10;
                    memory[I+2] = reg[X] % 10;
                    PC += 2;
                    break;
                case 0x0055:
                    for(int i = 0; i <= X; i++){
                        memory[I + i] = reg[i];
                    }
                    PC += 2;
                    break;
                case 0x0065:
                    for(int i = 0; i <= X; i++){
                        reg[i] = memory[I + i];
                    }
                    PC += 2;
                    break;
                default:
                    std::cout << "INVALID" << std::endl;
                    break;
            }
            break;
        default:
            std::cout << "INVALID" << std::endl;
            break;
    }
    if (delayTimer > 0)
        delayTimer--;
    if (soundTimer > 0)
        soundTimer--;
}