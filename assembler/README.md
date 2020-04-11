# CHIP-8 Assembler
This project is a complete implementation of an assembler based on the set of mnemonics for the CHIP8 instruction set.
The primary inspiration for this project was to make writing CHIP-8 games for the emulator a little easier. This is acheived 
by the fact that mnemonics are much easier to read then binary, but also with the use of helpful error messages, as well as 
the inclusion of labels (making refactoring code much easier). 

## How It Works
The assembler is broken up into two separate parts, the first being the lexer/scanner which actually parses each token of input from the file 
and ensures that is a valid token in the assembly language. The second component is what actually converts your tokens into machine code.

**The Lexer**  
The Lexer was implemented using a DFA (*Deterministic Finite Automata*). This is essentially a state machine. The lexer goes 
through the input file and each character takes us to a new state based on our previous state. When we hit an error state, we 
take the last state we were at and that is our token. If this token is valid for our language, we accept it, otherwise an error
is displayed. The use of a DFA for lexing prevents tight constraints on how the author must type their code. It is also extremely 
simple to refactor.

**The Builder**  
Our assembler is a 2-Pass Assembler. What this means is that we will loop over the assembly code twice. The first loop will be
to look for all label definitions and build our symbol table. Then in the second pass we will generate our machine code, replacing
any label references with their mapped memory address in the symbol table. The primary benefit to a 2-Pass assembler is that it 
enables the forward referencing of labels.

## How To Use
1. Compile the assembler by entering this director and typing `make`.
2. Write your source code in any file.
3. Run `./chipAssembler <path_to_src> <path_to_output>`.
4. Use the optional parameter `-st` at the end to print out the symbol table after assembly.

## Mnemonics Table  
All `0xnnn` can be replaced with a label name, and all numbers must be written in hex.  
Comments are to be written on independent lines and are commenced with the `#` character.  
View [Cowgods Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#1nnn) for what each opcode does.

| Mnemonic      | Opcode      
| ------------- |:------------- 
| CLS           | 0x00E0
| RET           | 0x00EE      
| JUMP 0xnnn    | 0x1nnn
| SKP $a 0xkk   | 0x3xkk
| SKPN $a 0xkk  | 0x4xkk      
| SKPR $a $b    | 0x5ab0
| LDC $a 0xkk   | 0x6xkk
| ADD $a 0xkk   | 0x7xkk   
| COPY $a $b    | 0x8ab0
| OR $a $b      | 0x8ab1
| AND $a $b     | 0x8ab2
| XOR $a $b     | 0x8ab3
| ADDC $a $b    | 0x8ab4
| SUB $a $b     | 0x8ab5     
| SHR $a $b     | 0x8ab6
| SUBC $a $b    | 0x8ab7
| SHL $a $b     | 0x8abe    
| SKPRN $a $b   | 0x9ab0
| LD 0xnnn      | 0x00E0
| JUMPI 0xnnn   | 0xBnnn      
| DRW $a $b 0xc | 0xDabc
| SKPK $a       | 0xea9e     
| SKPKN $b      | 0xeba1
| LDT $a        | 0xfa07
| KEY $a        | 0xfa07
| SETT $a       | 0xfa07
| SETS $a       | 0xfa07
| ADDI $a       | 0xfa07
| LDSP $a       |  0xfa07
| BCD $a        | 0xfa07
| SAVE $a       | 0xfa07
| LDS $a        | 0xfa07

