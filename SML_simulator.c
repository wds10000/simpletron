//  *****************************************************************************
//  *
//  *    SML_simulator.c -- Source file for functions used by 'simpletron.c'.
//  *    Date Created: Mon Aug 17 15:14:05 2020
//  *
//  *****************************************************************************

#include "SML_simulator.h"

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
  int success = 0; // Indicates if program file was loaded successfully.
  
  // Declare pointers.
  unsigned int *instr_reg_ptr;
  unsigned int *opern_cod_ptr;
  int *instr_cnt_ptr;
  int *accum_ptr;
  int *oprnd_ptr;
  Print_Struct_Ptr prnt_strct_ptr = NULL;
  
  instr_reg_ptr = &instr_reg;
  opern_cod_ptr = &opern_cod;
  instr_cnt_ptr = &instr_cnt;
  accum_ptr = &accum;
  oprnd_ptr = &oprnd;


  // Load a program from file into memory.
  success = load_file(mem, program_name, instr_cnt_ptr, &prnt_strct_ptr);

  // If file was loaded successfully, execute loaded program.
  if (success == 1) {
    execute(mem, instr_reg_ptr, opern_cod_ptr,
	    instr_cnt_ptr, accum_ptr, oprnd_ptr, &prnt_strct_ptr);
  }

  // If an input file was not successfully loaded, assign 'no_input' to
  // 'program_name'.
  if (success == 0) {
    program_name = "no_input.txt";
  }
  // Print output to screen and save to file.
  print_output(&prnt_strct_ptr, program_name);
}

//  *****************************************************************************
//  ***                     Function 'load_file'                              ***
//  *****************************************************************************
int load_file(int memory[], char program_name[],
	      int *instruction_counter_ptr, Print_Struct_Ptr *print_struct_ptr)
{
  FILE *load_ptr; // Pointer to file containing program instructions.

  // Open file chosen by user (read mode "r+").
  if ((load_ptr = fopen(program_name, "r+")) != NULL) { 
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
      fscanf(load_ptr, "%d\n", &memory[i]); // Load instruction to memory.
    } 
    fclose(load_ptr); // Close file 'program_name'.
    return 1;
  }
  // Open file chosen by user (read mode "r+").  
  // If file 'program_name' cannot be opened, print error message.
  else {
    printf("%s\n", "Program File Could Not Be Opened.");

    // Pass warning to 'add_struct_members' to faciliate printing.
    char temp[PROMPT_LENGTH] = "Program Could Not Be Opened";
    add_struct_members(temp, 99, print_struct_ptr);
    
    return 0;
  }
}

//  *****************************************************************************
//  ***                     Function 'execute'                                ***
//  *****************************************************************************
void execute(int memory[], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr,
	     Print_Struct_Ptr *print_struct_ptr)
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
      read_instruction(memory, operand_ptr, print_struct_ptr);
      break;
    case 11: // 'write_instruction' function call.
      write_instruction(memory, operand_ptr, print_struct_ptr);
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
      divide_instruction(memory, operand_ptr, accumulator_ptr, terminate_ptr,
			 print_struct_ptr);
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
      default_instruction(terminate_ptr, print_struct_ptr);
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
void read_instruction(int memory[], int *operand_ptr,
		      Print_Struct_Ptr *print_struct_ptr)
{
  // Print prompt and enter data.
  printf("%-2c", '?');
  scanf("%d", &memory[*operand_ptr]);

  // Pass prompt and data to 'add_struct_members' to faciliate printing.
  char temp[PROMPT_LENGTH] = "? ";
  add_struct_members(temp, memory[*operand_ptr], print_struct_ptr);
}

//  *****************************************************************************
//  ***                     Function 'write_instruction'                      ***
//  *****************************************************************************
void write_instruction(int memory[], int *operand_ptr,
		       Print_Struct_Ptr *print_struct_ptr)
{
  // Print prompt and  data.
  printf("%-2c%d\n", '>', memory[*operand_ptr]);

  char temp[PROMPT_LENGTH] = "> ";
  // Pass prompt and data to 'add_struct_members' to faciliate printing.
  add_struct_members(temp, memory[*operand_ptr], print_struct_ptr);
}

//  *****************************************************************************
//  ***                     Function 'load_instruction'                       ***
//  *****************************************************************************
void load_instruction(int memory[], int *operand_ptr,
		      int *accumulator_ptr)
{
  *accumulator_ptr = memory[*operand_ptr];  
}

//  *****************************************************************************
//  ***                     Function 'store_instruction'                      ***
//  *****************************************************************************
void store_instruction(int memory[], int *operand_ptr,
		       int *accumulator_ptr)
{
  memory[*operand_ptr] = *accumulator_ptr;
}

//  *****************************************************************************
//  ***                     Function 'add_instruction'                        ***
//  *****************************************************************************
void add_instruction(int memory[], int *operand_ptr,
		     int *accumulator_ptr)
{
  *accumulator_ptr += memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                  Function 'subtract_instruction'                      ***
//  *****************************************************************************
void subtract_instruction(int memory[], int *operand_ptr,
			  int *accumulator_ptr)
{
  *accumulator_ptr -= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                     Function 'divide_instruction'                     ***
//  *****************************************************************************
void divide_instruction(int memory[], int *operand_ptr,
			int *accumulator_ptr, int *terminate_ptr,
			Print_Struct_Ptr *print_struct_ptr)
{
  if (memory[*operand_ptr] == 0){
    printf("%s\n%s\n",
	   "Attempt to Divide by Zero. ",
	   "Simpletron Execution Abnormally Terminated.");

    // Pass warning to 'add_struct_members' to faciliate printing.
    char temp1[PROMPT_LENGTH] = "Attempt to Divide by Zero.\n";
    add_struct_members(temp1, 99, print_struct_ptr);
    char temp2[PROMPT_LENGTH] = "Simpletron Execution Abnormally Terminated.\n";
    add_struct_members(temp2, 99, print_struct_ptr);
    *terminate_ptr = 1;
  }
  else {
    *accumulator_ptr /= memory[*operand_ptr];
  }
} 

//  *****************************************************************************
//  ***                  Function 'multiply_instruction'                      ***
//  *****************************************************************************
void multiply_instruction(int memory[], int *operand_ptr,
			  int *accumulator_ptr)
{
  *accumulator_ptr *= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                  Function 'remainder_instruction'                     ***
//  *****************************************************************************
void remainder_instruction(int memory[], int *operand_ptr,
			   int *accumulator_ptr)
{
  *accumulator_ptr %= memory[*operand_ptr];
}

//  *****************************************************************************
//  ***                Function 'exponentiation_instruction'                  ***
//  *****************************************************************************
void exponentiation_instruction(int memory[], int *operand_ptr,
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
void default_instruction(int *terminate_ptr, Print_Struct_Ptr *print_struct_ptr)
{
  printf("%s\n%s\n",
	 "Invalid Instruction Detected.",
	 "Simpletron Execution Abnormally Terminated.");

  // Pass warning to 'add_struct_members' to faciliate printing.
  char temp1[PROMPT_LENGTH] = "Invalid Instruction Detected.\n";  
  add_struct_members(temp1, 99, print_struct_ptr);
  char temp2[PROMPT_LENGTH] = "Simpletron Execution Abnormally Terminated.\n";
  add_struct_members(temp2, 99, print_struct_ptr);
  *terminate_ptr = 1;
}

//  *****************************************************************************
//  ***                     Function 'load_register'                          ***
//  *****************************************************************************
void load_register(int memory[], int *instruction_counter_ptr,
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

//  *****************************************************************************
//  ***                     Function 'add_struct_members'                     ***
//  *****************************************************************************
void add_struct_members(char prompt_member[], int value_member,
			Print_Struct_Ptr *print_struct_ptr)
{
  // Use malloc to allocate memory to 'Print_Struct' object.
  Print_Struct_Ptr new_struct_ptr = malloc(sizeof(Print_Struct));

  // If memory was allocated, populate the struct.
  if (new_struct_ptr != NULL) {
    strcpy(new_struct_ptr->prompt, prompt_member);
    new_struct_ptr->value = value_member;
    new_struct_ptr->next_struct_ptr = NULL;

    // If this struct points to another, reassign 'next_struct_ptr' and
    // 'print_struct_ptr'.
    if (new_struct_ptr->next_struct_ptr == NULL) {
      new_struct_ptr->next_struct_ptr = *print_struct_ptr;
      *print_struct_ptr = new_struct_ptr;
    }
  }
  // If memory was not allocated, print error message.
  else {
    printf("%s\n", "Memory could not be allocated.");
  }
}

//  *****************************************************************************
//  ***                     Function 'print_output'                           ***
//  *****************************************************************************
void print_output(Print_Struct_Ptr *print_struct_ptr, char* program_name)
{
  FILE *output_ptr; // Pointer to file containing program output.
  char output_file[100] = "(output)_"; // Name of the file output is saved to
                                       // (add 5 so ".txt" & '\0' can be added).
  unsigned int length = strlen(program_name); // Length of 'program_name'.

  // Append 'program_name' to '(output)_".
  for (size_t i = 0; i < length; i++){
    output_file[i + 9] = program_name[i];
  }
  
  //Create output file (read mode "r+").
  if ((output_ptr = fopen(output_file, "w+")) != NULL) {

    //Iterate through 'print_array_ptr', print each element to 'output_file'.
    print_struct(print_struct_ptr, output_ptr);
    fclose(output_ptr); // Close 'output_file'.
  }
  // If file 'output_file' cannot be created, print error message.
  else {
    printf("%s\n", "Output File Could Not Be Created.");
  }
}

//  *****************************************************************************
//  ***                     Function 'print_struct'                           ***
//  *****************************************************************************
void print_struct(Print_Struct_Ptr *print_struct_ptr, FILE *file_ptr)
{
  // Use recursion to print the members of the list of structs to file in
  // reverse order.

  // If the current struct does not point to another, print it's members.
  if ((*print_struct_ptr)->next_struct_ptr == NULL){
    fprintf(file_ptr, "%s", (*print_struct_ptr)->prompt);

    // Don't print the value member of the struct if = 99.
    if ((*print_struct_ptr)->value != 99) {
      fprintf(file_ptr, "%d\n", (*print_struct_ptr)->value);
    }
  }
  // If the current struct points to another, call 'print_struct' again and
  // print the members of the current struct.
  else {
    Print_Struct_Ptr temp_ptr;
    temp_ptr = (*print_struct_ptr)->next_struct_ptr;
    print_struct(&temp_ptr, file_ptr);
    fprintf(file_ptr, "%s", (*print_struct_ptr)->prompt);

    // Don't print the value member of the struct if = 99.
    if ((*print_struct_ptr)->value != 99) {
      fprintf(file_ptr, "%d\n", (*print_struct_ptr)->value);
    }
  }
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
