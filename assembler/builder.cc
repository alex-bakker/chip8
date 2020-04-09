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

uint8_t getRegisterNum(Token token) {
    return stoi(token.getLexeme().substr(1));
}

bool checkRegister(Token token) {
    if(token.getKind() != Token::Kind::VARIABLE){
        std::cerr << "Error, expected VARIABLE token\n";
        return false;
    }
    
    int var = getRegisterNum(token);
    if(var < 0 || var > 0xF) {
        std::cerr << "Error, " << var << " is not a valid register.\n";
        return false;
    }
    return true;
}

bool buildZeroCommands(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
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
            std::cerr << "Error, reference to label " << label << " that has not yet been defined.\n";
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

    opcodes.push_back(opcode | address);
    return true;
}

bool buildRegNum(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 3, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;

    //Check bounds of the two bit numbers
    int num = tokens.at(2).getValue();
    if(num == -1){
        std::cerr << "Error, expected a NUMBER not " << tokens.at(2).getLexeme();
        return false;
    }

    if(num > UINT8_MAX || num < 0) {
        std::cerr << "Error, " << tokens.at(2).getLexeme() << " is not a valid 8 bit NUMBER\n";
        return false;
    }

    uint16_t opcode = (uint16_t)num;
    uint8_t reg = getRegisterNum(tokens.at(1));
    
    opcode |= (reg << 8);

    std::string cmd = tokens.at(0).getLexeme();
    uint8_t prefix; 
    if(cmd == "SKP") prefix = 0x3;
    else if(cmd == "SKPN") prefix = 0x4;
    else if(cmd == "LDC") prefix = 0x6;
    else if(cmd == "ADD") prefix = 0x7;
    else prefix == 0xC;

    opcodes.push_back(opcode | (prefix << 12));
    return true;
}

bool buildTwoReg(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 3, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;
    if(!checkRegister(tokens.at(2))) return false;

    uint8_t regOne = getRegisterNum(tokens.at(1));
    uint8_t regTwo = getRegisterNum(tokens.at(2));
    uint8_t mid = (regOne << 4) | regTwo;

    std::string cmd = tokens.at(0).getLexeme();
    uint16_t opcode = 0;

    if(cmd == "SKPR") opcode |= (0x5 << 12);
    else opcode |= (0x9 << 12);

    opcodes.push_back(opcode | (mid << 4));
    return true;
}

bool buildTwoRegEight(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 3, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;
    if(!checkRegister(tokens.at(2))) return false;

    uint8_t regOne = getRegisterNum(tokens.at(1));
    uint8_t regTwo = getRegisterNum(tokens.at(2));
    uint8_t mid = (regOne << 4) | regTwo;

    std::string cmd = tokens.at(0).getLexeme();
    uint16_t opcode = 0;

    if(cmd == "COPY") opcode = 0x0;
    else if(cmd == "OR") opcode = 0x1;
    else if(cmd == "AND") opcode = 0x2;
    else if(cmd == "XOR") opcode = 0x3;
    else if(cmd == "ADDC") opcode = 0x4;
    else if(cmd == "SUB") opcode = 0x5;
    else if(cmd == "SHR") opcode = 0x6;
    else if(cmd == "SUBC") opcode = 0x7;
    else if(cmd == "SHL") opcode = 0x8;
    else opcode == 0xe;

    opcode |= (mid << 4);
    opcodes.push_back(opcode | (0x8 << 12));
    return true;
}

bool buildTwoRegNum(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 4, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;
    if(!checkRegister(tokens.at(2))) return false;

    uint8_t num = tokens.at(3).getValue();
    if(num == -1){
        std::cerr << "Error, the 3rd operand should be a hex number, not " << tokens.at(3).getLexeme() << std::endl;
        return false;
    } else if(num > 0xF){
        std::cerr << "Error, " << tokens.at(3).getLexeme() << " is too large." << std::endl;
        return false;
    }

    uint8_t regOne = getRegisterNum(tokens.at(1));
    uint8_t regTwo = getRegisterNum(tokens.at(2));
    uint8_t mid = (regOne << 4) | regTwo;

    std::string cmd = tokens.at(0).getLexeme();
    uint16_t opcode = num;
    opcode |= (mid << 4);
    opcodes.push_back(opcode | (0xD << 12));
    return true;
}

bool buildKey(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 2, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;
    uint8_t reg = getRegisterNum(tokens.at(1));

    uint16_t opcode;
    std::string cmd = tokens.at(0).getLexeme();
    if(cmd == "SKPK") opcode = 0x9E;
    else opcode = 0xA1;

    opcode |= (reg << 8);
    opcodes.push_back(opcode | (0xE << 12));
    return true;
}

bool buildReg(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes) {
    if(!checkTokenCount(tokens.at(0), 2, tokens.size())) return false;
    if(!checkRegister(tokens.at(1))) return false;
    uint8_t reg = getRegisterNum(tokens.at(1));
    uint16_t opcode;
    
    std::string cmd = tokens.at(0).getLexeme();
    if(cmd == "LDT") opcode = 0x7;
    else if(cmd == "KEY") opcode = 0xA;
    else if(cmd == "SETT") opcode = 0x15;
    else if(cmd == "SETS") opcode = 0x18;
    else if(cmd == "ADDI") opcode = 0x1E;
    else if(cmd == "LDSP") opcode = 0x29;
    else opcode = 0x33;

    opcode |= (reg << 8);
    opcodes.push_back(opcode | (0xF << 12));
    return true;
}