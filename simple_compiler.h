//  *****************************************************************************
//  *
//  *    simple_compiler.h -- Header file for 'simple_compiler.c'.
//  *    Author: Wade Shiell
//  *    Date Created: Wed Aug 26 10:02:21 2020
//  *
//  *****************************************************************************

#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYMBOL_SIZE 100 // Size of the symbol table.
#define COMPILER_MEM_SIZE 100 // Size of the compiler memory.
#define SML_MEM_SIZE 100 // Size of Simpletron memory.

// Enumeration for 'simple' keywords and branch statements.
// REMARK - Line is a remark.
// CONST - Constant stored in given element ofsymbol table.
// ZERO - Zero stored in given element ofsymbol table.
// VARIABLE - Variable stored in given element ofsymbol table.
// LINE - Line number stored in given element ofsymbol table.
// INPUT - Requests user input.
// PRINT - Program prints output.
// LET - Assignment made to variable.
// IF - Start of branch statement.
// LESS_THAN - 'if x < y, then'.
// LESS_EQUAL - 'if x <= y, then'.
// EQUAL -'if x == y, then'.
// GREATER_EQUAL - 'if x >= y, then'.
// GREATER_THAN - 'if x > y, then'.
// GOTO - Program branches to another line.
// END - Program terminates execution.

enum simple_modes {REMARK, CONST, ZERO, VARIABLE, LINE, INPUT, PRINT, LET, IF,
		   LESS_THAN, LESS_EQUAL, EQUAL, GREATER_EQUAL, GREATER_THAN,
		   GOTO, END};

// Enumeration for 'SML' codes.
// NIL - Empty enumeration element.
// READ - Reads input from user.
// WRITE - Outputs data from memory.
// LOAD - Loads data from memory into accumulator.
// STORE - Stores data from accumulator to memory.
// ADD - Adds data from specified memory location to accumulator.
// SUBTRACT - Subtracts data in specified memory location from accumulator.
// MULTIPLY - Multiplies data from specified memory location with accumulator.
// DIVIDE - Divides data in accumulator by data in specific memory location.
// REMAINDER - Divides accumulator data by data in memory and returns remainder.
// EXPONENT - Raises data in accumulator the the power of data from memory.
// BRANCH - Branches to a specific location in memory.
// BRANCHNEG - Branches to a specific location in memory if accumulator < 0.
// BRANCHNEG - Branches to a specific location in memory if accumulator = 0.
// HALT - Terminates program execution.

enum SML_codes {NIL, READ = 10, WRITE = 11, LOAD = 20, STORE = 21, ADD = 30,
		SUBTRACT = 31, DIVIDE = 32, MULTIPLY = 33, REMAINDER = 34,
		EXPONENT = 35, BRANCH = 40, BRANCHNEG = 41, BRANCHZERO = 42,
		HALT = 43};

// Defines 'symbols' struct to hold variables associated with symbol table.
struct symbols {
  int symbol; // Value of struct data.
  char type; // Type of struct data (constant 'C', variable 'V', location 'L').
  int location; // Location of data in SML memory.
  bool is_empty; // Indicates whether current struct is empty.
};

// typedefs.
typedef struct symbols Symbols;
typedef Symbols *Symbols_Ptr;

// Compiles the loaded 'simple' program and saves the resulting 'SML' code
// to file.
void compile(char simple_file[]);

// Initialises arrays 'compiler_memory', 'symbol_table' and 'flag'.
void initialise(char *compiler_memory[], unsigned int SML_memory[],
		Symbols symbol_table[],	int flag[]);

// Loads a 'simple' program from file into the compiler.
void load_program(char *compiler_memory[], char simple_file[]);

// Performs first pass on 'simple' program statements.
void first_pass(unsigned int SML_memory[], char *compiler_memory[],
		Symbols symbol_table[], int flag[],
		unsigned int instruction_counter);

// Performs second pass on 'simple' program statements.
void second_pass(unsigned int SML_memory[], char *compiler_memory[],
		 Symbols symbol_table[], int flag[], char simple_file[]);

// Parses the string passed to the function into tokens.
char** tokenise(char **tokenised_string, char *simple_statement);

// Compares a token to the various 'simple' keywords, variables and constants.
unsigned int compare_token(char *token, unsigned int token_position,
			   unsigned int SML_memory[], char *compiler_memory[],
			   Symbols symbol_table[], int flag[],
			   unsigned int *memory_location_ptr,
			   unsigned int *table_index_ptr, bool go_to);

// Search symbol table. If symbol is found, return 1 otherwise return 0.
unsigned int search_table(Symbols symbol_table[], unsigned int symbol_code,
			  char *token);

// Add the passed token to 'symbol_table' if not already added.
unsigned int add_symbol(unsigned int token_value, char *token,
			Symbols symbol_table[], unsigned int *symbol_counter,
			bool go_to);

// Search through 'flag' and replace any unresolved references in 'SML_memory'.
void replace_references(int flag[], unsigned int SML_memory[],
			Symbols symbol_table[]);

// Saves compiled 'SML' code to disk.
void save_file(unsigned int SML_memory[], char simple_file[]);

// Need to define this function.
void convert_infix(void);

// Need to define this function.
void evaluate_postfix(void);

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
