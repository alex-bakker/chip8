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