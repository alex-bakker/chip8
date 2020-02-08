#include <stack>
#include <string>

class Chip8 {
    private:
        //60 Hertz timers
        uint8_t delayTimer;
        uint8_t soundTimer;

        //Special 16 bit registers
        uint16_t I;
        uint16_t PC;

        //General 8-bit registers
        uint8_t reg[16];

        //Virtual memory
        uint8_t memory[4096];

        //Hold the current opcode
        uint16_t opcode;

        //Stack functionality for procedure calls
        std::stack<uint16_t> stack;

        //Store the current state of the key
        uint8_t keyboard[16];

    public:

        //Holds the current screen
        uint8_t display[64 * 32];

        static const int MEM_START;
        bool drawFlag;
        bool soundFlag;
        void cycle();
        void init();
        void loadRom(std::string rom);
        void updateKey(int index, uint8_t val);
        Chip8(std::string rom);
};