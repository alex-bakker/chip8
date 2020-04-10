#include <vector>
#include <string>
#include <set>
#include <map>
#include "Token.h"

class DFA {
    private:
        /** 
         * Enum to represent all states in the DFA.
         */ 
        enum State {
            ID = 0,
            LABEL,
            VARIABLE,
            NUM,
            COMMENT,
            WHITESPACE,
            ZERO,

            ZEROX,
            START,
            DOLLAR, 
            ERROR,

            //Not a state, used to keep track of the number of states
            NUM_STATES 
        };

        /**
         * 2D Array represent the transition function
         * This is a function of OldState and a single Char to output the new State.
         */
        State transitionFunction[NUM_STATES][128];
        
        /**
         * Set of accepting states.
         * Used once an ERROR state is hit to check if current state is accepting.
         */
        std::set<State> acceptingStates;

        /**
         * Mapping from State to Kind
         */
        std::map<State, Token::Kind> kindFromState;

        /**
         * All transitions lead to an ERROR state by default.
         */
        void initializeTransitionFunction();

        /**
         * Inserts a mapping for the Kind From State
         */
        void addKindFromStateMapping(State state, Token::Kind kind);

        /**
         * Create a transition in our transitionFunction from oldState to newState 
         * for all characters in the input 
         */
        void createTransition(State oldState, const std::string &input, State newState);

        /**
         * Create a transition in our transitionFunction from oldState to newState
         * for every ascii char based on some test function.
         */
        void createTransition(State oldState, int (*compare)(int), State newState);

        /**
         * Returns true if the given state == ERROR.
         */
        bool isErrorState(State state);

        /**
         * Returns true if the given state is in the set of AcceptingStates
         */
        bool isAcceptingState(State state);
        
        /**
         * Returns the State after taking a transition
         */
        State takeTransition(State state, char c);

    public:
        /**
         * Takes in a single line and outputs a list tokens. Removes WHITESPACE and COMMENTS from this list
        */
        std::vector<Token> simplifiedMaximalMunch(const std::string &line);
        void printTransitions();
        DFA();
};
