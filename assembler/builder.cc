#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Token.h"

bool checkTokenCount(Token cmd, int expectedLength, int length) {
    if(length != expectedLength){
        std::cerr << "Error, command " << cmd.getLexeme() << " should have " << expectedLength - 1 << " operands. Not " << length - 1 << ".\n";
        return false;
    }
    return true;
}

bool buildZeroCommands(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    uint16_t opcode = 0;
    if(!checkTokenCount(tokens.at(0), 1, tokens.size())) return false;

    if(tokens.at(0).getLexeme() == "CLS")
        opcodes.push_back((uint16_t)0x00E0);
    else
        opcodes.push_back((uint16_t)0x00EE);
    return true;
}

bool buildMemAddress(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes, std::map<std::string, int> &st) {
    uint16_t opcode = 0;
    std::string cmd = tokens.at(0).getLexeme();
    int address = 0;

    if(!checkTokenCount(tokens.at(0), 2, tokens.size())) return false;

    if(tokens.at(1).getKind() == Token::Kind::ID) {
        std::string label = tokens.at(1).getLexeme();
        if(st.count(label) > 0){
            address = st.at(label);
        } else {
            std::cerr << "Error, reference label " << label << " that has not yet been defined.\n";
            return false;
        }
    } else if(tokens.at(1).getKind() == Token::Kind::NUM){
        address = tokens.at(1).getValue();
    } else {
        std::cerr << "Error, command " << cmd << " should have 1 operand. Either a memory address or a label.\n";
        return false;
    }

    if(address < 0 || address > 0xFFF){
        std:: cerr << "Invalid address, must be within 0 - 0xFFF\n";
        return false;
    } else if (address % 2 != 0) {
        std::cerr << "Invalid data access, address must be divisible by two.\n";
        return false;
    }

    uint16_t validAddr = address;
    if(cmd == "JUMP") opcode |= (0x1 << 12);
    else if(cmd == "CALL") opcode |= (0x2 << 12);
    else if(cmd == "LD") opcode |= (0xA << 12);
    else if(cmd == "JUMPI") opcode |= (0xB << 12);

    opcode |= address;
    opcodes.push_back(opcode);
    std::cout<< "OPCODE VALUE = " << opcode << std::endl;
    return true;
}
