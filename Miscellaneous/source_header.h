//  *****************************************************************************
//  *
//  *    source_header.h -- 
//  *    Author: Wade Shiell
//  *    Date Created: Mon Aug 17 15:08:55 2020
//  *
//  *****************************************************************************

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MEMORY_SIZE 1000 // Define size of Simpletron 'memory'

enum mode_list {NOTHING, VIEW, DELETE, LOAD_MAN, LOAD_FILE, SAVE_FILE,
		VIEW_INSTR, EDIT, RUN, PR_REG, PR_MEM, PR_DUMP, SAVE_DUMP, QUIT};

// Runs Simpletron program on behalf of main.
void run_simpletron(void);

// Prints a list of all SML programs currently saved to file.
void view_programs(int memory[MEMORY_SIZE]);

// Deletes a chosen program from the current list of SML programs.
void delete_program(void);

// Loads instructions entered by user into Simpletron 'memory'.
void load_manual(int memory[MEMORY_SIZE], int *instruction_counter_ptr);

// Load program from file.
void load_file(int memory[MEMORY_SIZE], int *instruction_counter_ptr);

// Saves program to file.
void save_file(int memory[MEMORY_SIZE], char *register_string,
	       int *instruction_counter_ptr);

// Prints the list of program instructions currently loaded in 'memory'.
void view_instructions(int memory[MEMORY_SIZE], int *instruction_counter_ptr);

// Edits the SML program currenly loaded in 'memory'.
void edit_program(int memory[MEMORY_SIZE], int *instruction_counter_ptr);

// Runs the instructions stored in Simpletron 'memory'.
void execute(int memory[MEMORY_SIZE], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr) ;

// Prints summary of program registers.
void print_registers(bool print_option, char* register_string,
		     int *accumulator_ptr, int *instruction_counter_ptr,
		     unsigned int *instruction_register_ptr,
		     unsigned int *operation_code_ptr, int *operand_ptr);

// Prints data stored in Simpletron 'memory'.
void print_memory_dump(int memory[MEMORY_SIZE], char* dump_file_ptr);

// Prints computer dump & register information.
void dump(int memory[MEMORY_SIZE], char* register_string, char* dump_file_ptr,
	  unsigned int *instruction_register_ptr,
	  unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	  int *accumulator_ptr, int *operand_ptr);

// Saves memory dump for currently loaded program to new file.
void save_dump(int memory[MEMORY_SIZE], char* dump_file_ptr, bool print_option);

// Prints welcome message at program start.
void print_welcome(void);

// Prints goodbye message at Simpletron termination.
void print_goodbye(void);

// Prompts the user to manually enter SML instructions.
void print_entry_prompt(void);

// Prompts user to enter instruction to be loaded into 'memory'.
void enter_instruction(int *instruction_ptr, int *location_number_ptr,
		       int *instruction_counter_ptr);

// Prints messsage if invalid instruction entered.
void invalid_entry(int *instruction_ptr, int *location_number_ptr,
		   int *instruction_counter_ptr);

// Colour functions.
void red(void); 
void blue(void);
void green(void);
void yellow(void);
void reset(void);

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
