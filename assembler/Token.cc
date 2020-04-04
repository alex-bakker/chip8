#include "Token.h"

Token::Token(Token::Kind kind, std::string lexeme) : kind(kind), lexeme(lexeme) {} 

Token::Kind Token::getKind() const {return kind;}
std::string Token::getLexeme() const {return lexeme;}