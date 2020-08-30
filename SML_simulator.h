//  *****************************************************************************
//  *
//  *    SML_simulator.h -- Header file for 'SML_simulator.c'
//  *    Date Created: Mon Aug 17 15:08:55 2020
//  *
//  *****************************************************************************

#ifndef SIMPLE_HEADER_H
#define SIMPLE_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MEMORY_SIZE 1000 // Define size of Simpletron 'memory'
#define PROMPT_LENGTH 50 // Number of characters stored in 'prompt[]'.

// Holds variables to be passed to function 'print_output'.
struct print_struct {
  char prompt[PROMPT_LENGTH]; // Holds prompt string.
  int value; // Holds value data.
  struct print_struct *next_struct_ptr; // Points to next struct in the list.
};

typedef struct print_struct Print_Struct;
typedef Print_Struct *Print_Struct_Ptr;

// Runs Simpletron program on behalf of main.
void run_simpletron(char program_name[]);

// Load program from file.
int load_file(int memory[], char file_name[],
	      int *instruction_counter_ptr, Print_Struct_Ptr *print_struct_ptr);

// Runs the instructions stored in Simpletron 'memory'.
void execute(int memory[], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr, Print_Struct_Ptr *print_struct_ptr);

// Reads an hexadecumal integer from the keyboard and stores in 'memory'.
void read_instruction(int memory[], int *operand_ptr,
		      Print_Struct_Ptr *print_struct_ptr);

// Writes an decimal integer from 'memory' and displays it on screen.
void write_instruction(int memory[], int *operand_ptr,
		       Print_Struct_Ptr *print_struct_ptr);

// Loads an instruction from 'memory' into the accumulator.
void load_instruction(int memory[], int *operand_ptr,
		      int *accumulator_ptr);

// Stores an instruction in the accumulator in 'memory'.
void store_instruction(int memory[], int *operand_ptr,
		       int *accumulator_ptr);

// Adds an instruction in 'memory' to the value in the accumulator.
void add_instruction(int memory[], int *operand_ptr,
		     int *accumulator_ptr);

// Subtracts an instruction in 'memory' from the value in the accumulator.
void subtract_instruction(int memory[], int *operand_ptr,
			  int *accumulator_ptr);

// Divides the value in the accumulator by a value from 'memory'.
void divide_instruction(int memory[], int *operand_ptr,
			int *accumulator_ptr, int *terminate_ptr,
			Print_Struct_Ptr *print_struct_ptr);

// Multiplies a value from 'memory' by the value in the accumulator.
void multiply_instruction(int memory[], int *operand_ptr,
			  int *accumulator_ptr);

// Gives the remainder of the accumulator value divided by 'memory' value.
void remainder_instruction(int memory[], int *operand_ptr,
			   int *accumulator_ptr);

// Raises the value in the accumulator to value from 'memory'.
void exponentiation_instruction(int memory[], int *operand_ptr,
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
void default_instruction(int *terminate_ptr,
			 Print_Struct_Ptr *print_struct_ptr);

// Loads operation type and operand into 'instruction_register' and 'operand'.
void load_register(int memory[], int *instruction_counter_ptr,
		   unsigned int *instruction_register_ptr,
		   unsigned int *operation_code_ptr, int *operand_ptr);

// Adds prompt and data to 'print_struct_ptr' members.
void add_struct_members(char prompt_member[], int value_member,
			Print_Struct_Ptr *print_struct_ptr);

// Prints program output to file.
void print_output(Print_Struct_Ptr *print_struct_ptr, char *program_name);

// Iterates through a struct and prints entries in reverse order.
void print_struct(Print_Struct_Ptr *print_struct_ptr, FILE *file_ptr);

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
