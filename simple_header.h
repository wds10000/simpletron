//  *****************************************************************************
//  *
//  *    simple_header.h -- 
//  *    Author: Wade Shiell
//  *    Date Created: Mon Aug 17 15:08:55 2020
//  *
//  *****************************************************************************

#ifndef SIMPLE_HEADER_H
#define SIMPLE_HEADER_H

#include <stdio.h>
#include <math.h>
#define MEMORY_SIZE 1000 // Define size of Simpletron 'memory' 

// Runs Simpletron program on behalf of main.
void run_simpletron(char program_name[]);

// Load program from file.
void load_file(int memory[MEMORY_SIZE], char file_name[],
	       int *instruction_counter_ptr);

// Runs the instructions stored in Simpletron 'memory'.
void execute(int memory[MEMORY_SIZE], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr) ;

// Reads an hexadecumal integer from the keyboard and stores in 'memory'.
void read_instruction(int memory[MEMORY_SIZE], int *operand_ptr);

// Writes an decimal integer from 'memory' and displays it on screen.
void write_instruction(int memory[MEMORY_SIZE], int *operand_ptr);

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

// Terminates execution if an abnormal instruction is detected.
void default_instruction(int *terminate_ptr);

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
