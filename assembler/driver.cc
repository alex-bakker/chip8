#include <string>
#include <iostream>
#include <fstream>
#include "DFA.h"
#include "Token.h"
#include "ScanningExceptions.h"

std::string getKindString(Token::Kind kind){
    switch (kind) {
    case Token::Kind::ID:
        return "ID";
        break;
    case Token::Kind::LABEL:
        return "Label";
        break;
    case Token::Kind::VARIABLE:
        return "VARIABLE";
        break;
    case Token::Kind::NUM:
        return "NUM";
        break;
    case Token::Kind::COMMENT:
        return "COMMENT";
        break;
    case Token::Kind::COMMA:
        return "COMMA";
        break;
    case Token::Kind::WHITESPACE:
        return "WHITESPACE";
        break;
    default:
        return "";
        break;
    }
}

void printToken(const Token &token) {
    std::cout << "(" << getKindString(token.getKind()) << ", " << "'" << token.getLexeme() << "') ";
}

std::map<std::string, int> symbolTable;

/**
 * TODO: Will organize these by there first hex value (1, 2, ...f) 
 */
bool synthesize(const std::vector<Token> &tokens) {
}

int main(int argc, char* argv[]) {
    //Verify the number of arguments are correct
    if(argc != 2){
        std::cout << "Incorrect arguments. Pass in filename as single argument.\n";
        return 1;
    }

    //Attempt to open the file; check of existence
    std::ifstream file(argv[1]);
    if(file.fail()){
        std::cout << "Could not open the file, make sure the name is correct.\n";
        return 1;
    }

    //File is open, time to tokenize.
    DFA dfa;
    std::string line;
    std::vector<Token> tokens;
    std::vector<std::vector<Token>> tokenizedProgram;
    int lineNumber = 0; //Lines in the program blank or not
    int memoryIndex = 0x200; //Used to track memory address of a label, start at 0x200

    while(std::getline(file, line)) {
        lineNumber++;
        try {
            //Get the tokens for the line
            //If the line is not blank, we will add to tokenizedProgram and continue
            tokens = dfa.simplifiedMaximalMunch(line);
            if(tokens.size() == 0)
                continue;

            Token token = tokens.at(0);
            if(token.getKind() == Token::Kind::LABEL) {
                std::string labelName = token.getLexeme().substr(0, token.getLexeme().size() - 1);
                if(symbolTable.count(labelName) > 0){
                    std::cout << "Error, duplicate definition of label: " << labelName << " at line " << lineNumber << std::endl;
                    return 1;
                } else {
                    symbolTable.emplace(labelName, memoryIndex);
                    tokens.erase(tokens.begin());   //Way to erase the first element since it's a label
                }
            }
            tokenizedProgram.push_back(tokens);
            //Only want to increase memory if this was a line with an instruction
            //If it was just a line with a label then if we were to increment memory index we would get 
            //a blank word in this memory address
            if(tokens.size() > 0)
                memoryIndex+=2;

        } catch(TokenError &e) {
            std::cout << "Error on line " << lineNumber << " " << e.what() << std::endl;
            return 1;
        }
    }

    //At this point we have correctly built a Symbol table and removed all Labels, Comments, and Whitespace from our tokenized program.
    for(auto const &tokens  : tokenizedProgram){
        if(!synthesize(tokens))
            return 1;
    }

    for(auto const &label : symbolTable) {
        std::cout << label.first << " : " << std::hex << "0x" << label.second << std::endl;
    }
}
