//  *****************************************************************************
//  *
//  *    simple_compiler.h -- Header file for 'simple_compiler.c'.
//  *    Author: Wade Shiell
//  *    Date Created: Wed Aug 26 10:02:21 2020
//  *
//  *****************************************************************************

#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYMBOL_SIZE 100 // Size of the symbol table.
#define COMPILER_MEM_SIZE 100 // Size of the compiler memory.

// NIL - Empty enumeration element.
// REM - Remark, ignored by compiler.
// INPUT - Corresponds to SML 'read'.
// LET - Correpsonds to SML 'write'.
// IF - Statement is equivalent to SML 'branch', 'branchneg' or 'branchzero'.
// LESS_THAN - Less than.
// LESS_EQUAL - Less than or equal to.
// EQUAL - Equal to.
// GREAT_EQUAL - Greater than or equal to.
// GREAT - Greater than.
// GOTO - Corresponds to SML 'branch'.
// END - Corresponds to SML 'halt.

// Enumeration of keywords present in 'simple' program.
enum operations {NIL, REM, INPUT, LET, IF, LESS_THAN, LESS_EQUAL, EQUAL,
		 GREAT_EQUAL, GREAT_THAN, GOTO, END};

// Defines 'symbols' struct to hold variables associated with symbol table.
struct symbols {
  int symbol;
  char type;
  int location;
};

// typedefs.
typedef struct symbols Symbols;
typedef Symbols *Symbols_Ptr;

// Compiles the loaded 'simple' program and saves the resulting 'SML' code
// to file.
void compile(char simple_file[]);

// Initialises arrays 'symbol_table' and 'flag'.
void initialise(char *compiler_memory[], Symbols symbols_table[], int flag[]);

// Loads a 'simple' program from file into the compiler.
void load_program(char *compiler_memory[], char simple_file[]);

// Performs first pass on 'simple' program statements.
void first_pass(char *compiler_memory[], Symbols symbols_table[], int flag[]);

// Performs second pass on 'simple' program statements.
void second_pass(char *compiler_memory[], Symbols symbols_table[], int flag[]);

// Saves compiled 'SML' code to disk.
void save_file(unsigned int SML_memory[], char simple_file[]);

void tokenise(char *simple_statement);

void compare_token(char *token);
#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
