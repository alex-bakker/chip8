#include <stack>
#include <string>

class Chip8 {
    private:
        //60 Hertz timers
        unsigned char delayTimer;
        unsigned char soundTimer;

        //Special 16 bit registers
        unsigned short I;
        unsigned short PC;

        //General 8-bit registers
        unsigned char reg[16];

        //Virtual memory
        unsigned char memory[4096];

        //Hold the current opcode
        unsigned short opcode;

        //Stack functionality for procedure calls
        std::stack<short> stack;

        //Store the current state of the key
        unsigned char keyboard[16];

    public:

        //Holds the current screen
        unsigned char display[64 * 32];

        static const int MEM_START;
        bool drawFlag;
        void cycle();
        void init();
        void loadRom(std::string rom);
        void updateKey(int index, char val);
        Chip8(std::string rom);
};