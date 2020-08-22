//  *****************************************************************************
//  *
//  *    execute_header.h -- 
//  *    Author: Wade Shiell
//  *    Date Created: Thu Aug 20 11:47:13 2020
//  *
//  *****************************************************************************

#ifndef EXECUTE_HEADER_H
#define EXECUTE_HEADER_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define REGISTER_LENGTH 1000 // Define maximum size of string 'register_string'.
#define FILE_LENGTH 20 // Define maximum length of program file name.
#define MEMORY_SIZE 1000 // Define size of Simpletron 'memory' 
#define SENTINEL -0XFFFFF // Define sentinel value.

// Reads an hexadecumal integer from the keyboard and stores in 'memory'.
void read_instruction(int memory[MEMORY_SIZE], int *operand_ptr);

// Writes an hexadecimal integer from 'memory' and displays it on screen.
void write_instruction(int memory[MEMORY_SIZE], int *operand_ptr);

// Reads a string from the keyboard and stores it in 'memory'.
void read_string(int memory[MEMORY_SIZE], int *operand_ptr);

// Writes a string from 'memory' and displays it on screen.
void write_string(int memory[MEMORY_SIZE], int *operand_ptr);

// Loads an instruction from 'memory' into the accumulator.
void load_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		      int *accumulator_ptr);

// Stores an instruction in the accumulator in 'memory'.
void store_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		       int *accumulator_ptr);

// Adds an instruction in 'memory' to the value in the accumulator.
void add_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		     int *accumulator_ptr);

// Subtracts an instruction in 'memory' from the value in the accumulator.
void subtract_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			  int *accumulator_ptr);

// Divides the value in the accumulator by a value from 'memory'.
void divide_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			int *accumulator_ptr, int *terminate_ptr);

// Multiplies a value from 'memory' by the value in the accumulator.
void multiply_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			  int *accumulator_ptr);

// Gives the remainder of the accumulator value divided by 'memory' value.
void remainder_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			   int *accumulator_ptr);

// Raises the value in the accumulator to value from 'memory'.
void exponentiation_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
				int *accumulator_ptr);

// Returns a newline character.
void newline(int memory[MEMORY_SIZE], int *operand_ptr);

// Branches to a 'memory' location.
void branch_instruction(int *instruction_counter_ptr,
			int *operand_ptr, int *toggle_ptr);

// Branches to a 'memory' location if the accumulator value is negative.
void branchneg_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			   int *operand_ptr, int *toggle_ptr);

// Branches to a 'memory' location if the accumulator value is zero.
void branchzero_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			    int *operand_ptr, int *toggle_ptr);

// Terminates execution of the program loaded in 'memory'.
void halt_instruction(int *terminate_ptr);

// Increments the value of the 'memory' location counter by one.
void increment_counter(int *toggle_ptr, int *instruction_counter_ptr);

// Loads operation type and operand into 'instruction_register' and 'operand'.
void load_register(int memory[MEMORY_SIZE], int *instruction_counter_ptr,
		   unsigned int *instruction_register_ptr,
		   unsigned int *operation_code_ptr, int *operand_ptr);
#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
