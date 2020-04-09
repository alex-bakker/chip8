#include <vector>
#include <ctype.h>
#include "DFA.h"
#include "Token.h"
#include "ScanningExceptions.h"
#include <iostream>

//Simply for convenience
#define Kind Token::Kind

DFA::DFA() {
    initializeTransitionFunction();

    //Initialize the set of Accepting States
    acceptingStates = {ID, LABEL, VARIABLE, COMMA, COMMENT, WHITESPACE, NUM, ZERO};

    //Create the mapping from State to Kind
    addKindFromStateMapping(ID, Kind::ID);
    addKindFromStateMapping(LABEL, Kind::LABEL);
    addKindFromStateMapping(NUM, Kind::NUM);
    addKindFromStateMapping(VARIABLE, Kind::VARIABLE);
    addKindFromStateMapping(COMMENT, Kind::COMMENT);
    addKindFromStateMapping(COMMA, Kind::COMMA);
    addKindFromStateMapping(WHITESPACE, Kind::WHITESPACE);
    addKindFromStateMapping(ZERO, Kind::ZERO);

    //Build the actual DFA transitions
    createTransition(START, isalpha, ID);
    createTransition(START, "123456789", NUM);
    createTransition(START, "#", COMMENT);
    createTransition(START, isspace, WHITESPACE);
    createTransition(START, ",", COMMA);
    createTransition(START, "$", DOLLAR);
    createTransition(START, "0", ZERO);
    createTransition(ID, isalpha, ID);
    createTransition(ID, ":", LABEL);
    createTransition(WHITESPACE, isspace, WHITESPACE);
    createTransition(DOLLAR, isdigit, VARIABLE);
    createTransition(VARIABLE, isdigit, VARIABLE);
    createTransition(COMMENT, [](int c) -> int {return c != '\n';}, COMMENT);
    createTransition(NUM, "0123456789abcdefABCDEF", NUM);
    createTransition(ZERO, "x", ZEROX);
    createTransition(ZEROX, "0123456789abcdefABCDEF", NUM);
}

void DFA::initializeTransitionFunction() {
    for(int i = 0; i < DFA::NUM_STATES; i++) {
        for(int j = 0; j < 128; j++) {
            transitionFunction[i][j] = ERROR;
        }
    }
}

void DFA::addKindFromStateMapping(State state, Kind kind) {
    kindFromState.insert(std::pair<State, Kind>(state, kind));
}

void DFA::createTransition(State oldState, const std::string &input, State newState) {
    for(char c : input) {
        transitionFunction[oldState][c] = newState;
    }
}

void DFA::createTransition(State oldState, int (*compare)(int), State newState) {
    for(int i = 0; i < 128; i++) {
    if(compare(i))
        transitionFunction[oldState][i] = newState;
    }
}

bool DFA::isErrorState(State state) {
    return state == ERROR;
}

bool DFA::isAcceptingState(State state) {
    return acceptingStates.count(state) > 0;
}

DFA::State DFA::takeTransition(State state, char c) {
    return transitionFunction[state][c]; 
}

void DFA::printTransitions(){
    for(int i = 0; i < DFA::NUM_STATES; i++) {
        for(int j = 0; j < 128; j++) {
            if(transitionFunction[i][j] != ERROR)
            std::cout << "State: " << i << " Takes: " << (char)j << " to " << transitionFunction[i][j] << std::endl;
        }
    }
}

std::vector<Token> DFA::simplifiedMaximalMunch(const std::string &line) {
    State state = START;
    std::string currentLexeme;
    std::vector<Token> tokens;

    for(std::string::const_iterator it = line.begin(); it != line.end();) {
        State oldState = state;
        state = takeTransition(oldState, *it);

        bool failed = isErrorState(state);
        
        if(!failed){
            oldState = state;
            currentLexeme += *it;
            it++;
        }

        if(it == line.end() || failed){
            if(isAcceptingState(oldState)){
                if(oldState != State::COMMENT && oldState != State::WHITESPACE)
                    tokens.push_back(Token(kindFromState.at(oldState), currentLexeme));
                currentLexeme.clear();
                state = START;
            } else {
                currentLexeme += *it;
                throw TokenError("ERROR: Could not parse token " + currentLexeme);
            }
        }
    }
    return tokens;
}
