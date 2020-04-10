#include <string>
#include <map>
#include <vector>
#include <set>
#include "Token.h"

/**
* Construct the opcode for commands with zero operands.
*/
bool buildZeroCommands(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/** 
 * Construct the opcode for commands with a single operand which is a memory address.
 */
bool buildMemAddress(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes, std::map<std::string, int> &st);

/**
 * Construct the opcode for commands with a single register followed by a 8 bit number.
 */
bool buildRegNum(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/**
 * Construct the opcode for commands with two registers.
 */
bool buildTwoReg(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/**
 * Construct the opcode for commands with two registers and whos first digit is 0x8.
 */
bool buildTwoRegEight(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/**
 * Construct the opcode for commands with 2 reg and a 4 bit number
 */
bool buildTwoRegNum(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/**
 * Construct the opcode for commands with a single register
 */
bool buildKey(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);

/**
 * Constrcut the opcode for commands with a single register and begin with 0xF
 */
bool buildReg(const std::vector<Token> &tokens, std::vector<uint16_t> &opcodes);
