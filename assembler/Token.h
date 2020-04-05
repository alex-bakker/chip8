#include <string>

#ifndef TOKEN_H
#define TOKEN_H

/**
 * Helpful class representation of a single Typed object
 * Stores this object(Token) as a pair storing both the Kind of object
 * that is typed as well as the actual text of the element.
 * An example would be as follows: 
 * (KIND: Label, LEXEME: "gameLoop:")
 */
class Token {
    public:
        //The kinds of Tokens recognized by the assembnler 
        enum Kind {
            ID,
            LABEL,
            VARIABLE,
            NUM,
            COMMENT,
            COMMA,
            WHITESPACE,
            ZERO
        };
        
        /**
         * Simple constructor
         * Sets both the kind and the lexeme of the token
         */
        Token(Kind kind, const std::string lexeme);

        Kind getKind() const;
        std::string getLexeme() const;
        int getValue() const;

    private:
        Kind kind;
        std::string lexeme;
};

#endif