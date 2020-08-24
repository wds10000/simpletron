//  *****************************************************************************
//  *
//  *    simple_source.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Mon Aug 17 15:14:05 2020
//  *
//  *****************************************************************************

#include "simple_header.h"

//  *****************************************************************************
//  ***                     Function 'run_simpletron'                         ***
//  *****************************************************************************
void run_simpletron(char program_name[])
{  
  // Declare program variables.
  unsigned int instr_reg = 0; // Holds the current instruction.
  unsigned int opern_cod = 0; // Indicates the current instruction type.
  int instr_cnt = 0; // Tracks memory location of current instruction.
  int accum = 0; // Accumulator registry.
  int oprnd = 0; // Indicates the memory location currently operated on.
  int mem[MEMORY_SIZE] = {0}; // Array which simulates Simpletron memory.

  // Declare pointers.
  unsigned int *instr_reg_ptr;
  unsigned int *opern_cod_ptr;
  int *instr_cnt_ptr;
  int *accum_ptr;
  int *oprnd_ptr;
  
  instr_reg_ptr = &instr_reg;
  opern_cod_ptr = &opern_cod;
  instr_cnt_ptr = &instr_cnt;
  accum_ptr = &accum;
  oprnd_ptr = &oprnd;

  // Load a program from file into memory.
  load_file(mem, program_name, instr_cnt_ptr);
  // Execute loaded program.
  execute(mem, instr_reg_ptr, opern_cod_ptr,
  	  instr_cnt_ptr, accum_ptr, oprnd_ptr);
}

//  *****************************************************************************
//  ***                     Function 'load_file'                              ***
//  *****************************************************************************
void load_file(int memory[MEMORY_SIZE], char program_name[],
	       int *instruction_counter_ptr)
{
  FILE *load_ptr; // Pointer to file containing program instructions.

  // Open file chosen by user (read mode "r+").
  if ((load_ptr = fopen(program_name, "r+")) != NULL) { 
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
      fscanf(load_ptr, "%d\n", &memory[i]); // Load instruction to memory.
    } 
    fclose(load_ptr); // Close file 'loaded_file'.
  }
  // If file 'loaded_file' cannot be opened, print error message.
  else {
    printf("%s\n", "Program File Could Not Be Opened");
  }
}

//  *****************************************************************************
//  ***                     Function 'execute'                                ***
//  *****************************************************************************
void execute(int memory[MEMORY_SIZE], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr)
{
  // Reset 'instruction_counter' after a set of program instructions is
  // executed, otherwise the next set of instructions will not start from the
  // beginning of memory.
  *instruction_counter_ptr = 0;

  // Load instruction into 'instruction_register' and seperate it into
  // 'operation_code' and 'operand'.
  load_register(memory, instruction_counter_ptr, instruction_register_ptr,
		operation_code_ptr, operand_ptr);

  int terminate = 0; // Variable to control when program execution stops.
  int *terminate_ptr; // Pointer to variable 'terminate'.
  terminate_ptr = &terminate;

  int toggle; // Controls whether instruction counter is incremented (should not
  // be incremented if branch instruction is processed).  
  int *toggle_ptr; // Pointer to variable 'toggle'.
  toggle_ptr = &toggle;

  // Execute instructions stored in Simpletron memory until value of
  // '*terminate_ptr' causes program to stop running.
  while (*terminate_ptr != 1) {
    *toggle_ptr = 0; // Zero toggle at beginning of each instruction.

    // Switch statement is used to differentiate between instruction types
    // and perform relevant function calls..
    switch (*operation_code_ptr) {

    case 10: // 'read_instruction' function call.
      read_instruction(memory, operand_ptr);
      break;
    case 11: // 'write_instruction' function call.
      write_instruction(memory, operand_ptr);
      break;
    case 20: // 'load_instruction' function call.
      load_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 21: // 'store_instruction' function call.
      store_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 30: // 'add_instruction' function call.
      add_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 31: // 'subtract_instruction' function call.
      subtract_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 32: // 'divide_instruction' function call.
      divide_instruction(memory, operand_ptr, accumulator_ptr, terminate_ptr);
      break;
    case 33: // 'multiply_instruction' function call.
      multiply_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 34: // 'remainder_instruction' function call.
      remainder_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 35: // 'exponentiation_instruction' function call.
      exponentiation_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 40: // 'branch_instruction' function call.
      branch_instruction(instruction_counter_ptr, operand_ptr, toggle_ptr);
      break;
    case 41: // 'branch if negative_instruction' function call.
      branchneg_instruction(accumulator_ptr, instruction_counter_ptr,
			    operand_ptr, toggle_ptr);
      break;
    case 42: // 'branch if zero_instruction' function call.
      branchzero_instruction(accumulator_ptr, instruction_counter_ptr,
			     operand_ptr, toggle_ptr);
      break;
    case 43: // 'Halt_instruction' function call.
      halt_instruction(terminate_ptr);
      break;
    default: // 'default_instruction' function call.
      default_instruction(terminate_ptr);
      break;
    }

    // If instruction is not a branch, increment the instruction counter.
    if (*toggle_ptr != 1) {
      (*instruction_counter_ptr)++;
    }

    // Load instruction into 'instruction_register' and seperate it into
    // 'operation_code' and 'operand'.
    load_register(memory, instruction_counter_ptr, instruction_register_ptr,
		  operation_code_ptr, operand_ptr);
  }
}

//  *****************************************************************************
//  ***                     Function 'read_instruction'                       ***
//  *****************************************************************************
void read_instruction(int memory[MEMORY_SIZE], int *operand_ptr)
{
  scanf("%d", &memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                     Function 'write_instruction'                      ***
//  *****************************************************************************
void write_instruction(int memory[MEMORY_SIZE], int *operand_ptr)
{
  printf("%d\n", memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                     Function 'load_instruction'                       ***
//  *****************************************************************************
void load_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		      int *accumulator_ptr)
{
  *accumulator_ptr = memory[*operand_ptr];  
}

//  *****************************************************************************
//  ***                     Function 'store_instruction'                      ***
//  *****************************************************************************
void store_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		       int *accumulator_ptr)
{
  memory[*operand_ptr] = *accumulator_ptr;
}

//  *****************************************************************************
//  ***                     Function 'add_instruction'                        ***
//  *****************************************************************************
void add_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
		     int *accumulator_ptr)
{
  *accumulator_ptr += memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                  Function 'subtract_instruction'                      ***
//  *****************************************************************************
void subtract_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			  int *accumulator_ptr)
{
  *accumulator_ptr -= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                     Function 'divide_instruction'                     ***
//  *****************************************************************************
void divide_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			int *accumulator_ptr, int *terminate_ptr)
{
  if (memory[*operand_ptr] == 0){
    printf("%s\n",  "Attempt to Divide by Zero. "
	   "Simpletron Execution Abnormally Terminated");
    *terminate_ptr = 1;
  }
  else {
    *accumulator_ptr /= memory[*operand_ptr];
  }
} 

//  *****************************************************************************
//  ***                  Function 'multiply_instruction'                      ***
//  *****************************************************************************
void multiply_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			  int *accumulator_ptr)
{
  *accumulator_ptr *= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                  Function 'remainder_instruction'                     ***
//  *****************************************************************************
void remainder_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
			   int *accumulator_ptr)
{
  *accumulator_ptr %= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                Function 'exponentiation_instruction'                  ***
//  *****************************************************************************
void exponentiation_instruction(int memory[MEMORY_SIZE], int *operand_ptr,
				int *accumulator_ptr)
{
  *accumulator_ptr = pow(*accumulator_ptr, memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                 Function 'branch_instruction'                         ***
//  *****************************************************************************
void branch_instruction(int *instruction_counter_ptr,
			int *operand_ptr, int *toggle_ptr)
{
  *instruction_counter_ptr = *operand_ptr;
  *toggle_ptr = 1;
}

//  *****************************************************************************
//  ***                 Function 'branchneg_instruction'                      ***
//  *****************************************************************************
void branchneg_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			   int *operand_ptr, int *toggle_ptr)
{
  if (*accumulator_ptr < 0) {
    *instruction_counter_ptr = *operand_ptr;
    *toggle_ptr = 1;
  }
}

//  *****************************************************************************
//  ***                Function 'branchzero_instruction'                      ***
//  *****************************************************************************
void branchzero_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			    int *operand_ptr, int *toggle_ptr)
{
  if (*accumulator_ptr == 0) {
    *instruction_counter_ptr = *operand_ptr;
    *toggle_ptr = 1;
  }
}

//  *****************************************************************************
//  ***                     Function 'halt_instruction'                       ***
//  *****************************************************************************
void halt_instruction(int *terminate_ptr)
{
  *terminate_ptr = 1;
}

//  *****************************************************************************
//  ***                     Function 'default_instruction'                    ***
//  *****************************************************************************
void default_instruction(int *terminate_ptr)
{
  printf("%s\n",  "Abnormal Instruction Detected"
	 "Simpletron Execution Abnormally Terminated");
  *terminate_ptr = 1;
}

//  *****************************************************************************
//  ***                     Function 'load_register'                          ***
//  *****************************************************************************
void load_register(int memory[MEMORY_SIZE], int *instruction_counter_ptr,
		   unsigned int *instruction_register_ptr,
		   unsigned int *operation_code_ptr, int *operand_ptr)
{
  // Load instruction register.  
  *instruction_register_ptr = memory[*instruction_counter_ptr];
  // Holds operation type.
  *operation_code_ptr = *instruction_register_ptr / 100;
  // Hold operand instruction acts 
  *operand_ptr = *instruction_register_ptr % 100; 
}
// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
