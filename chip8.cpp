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
    opcode = memory[PC] << 8 | memory[pc + 1];
}