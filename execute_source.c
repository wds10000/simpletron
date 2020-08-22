//  *****************************************************************************
//  *
//  *    execute_source.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Thu Aug 20 11:48:15 2020
//  *
//  *****************************************************************************

#include "execute_header.h"

//  *****************************************************************************
//  ***                     Function 'read_instruction'                       ***
//  *****************************************************************************
void read_instruction(int memory[MEMORY_SIZE], int *operand_ptr)
{
  printf("%s%s\n%s", "***                -Enter hexadecimal integer-",
	 "                   ***",
	 "                              >> ");
  scanf("%X", &memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                     Function 'write_instruction'                      ***
//  *****************************************************************************
void write_instruction(int memory[MEMORY_SIZE], int *operand_ptr)
{
  printf("%s%s\n%s%X\n", "***                          -Value-",
	 "                             ***",
	 "                              >> ", memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                     Function 'read_string'                            ***
//  *****************************************************************************
void read_string(int memory[MEMORY_SIZE], int *operand_ptr)
{
  char str[99]; // Declare 'str' to hold user-entered string.
  unsigned int current = 1; // Current character number;
  unsigned int str_length = 0; // Length of user-entered string.
  unsigned int str_iter = 0; // Controls iteration through string.
  
  // Prompt user to enter a string of less than 98 characters.
  printf("%s%d%s%s\n%s", "***               -Enter string (max. size - ",
	 98, ")-", "                ***",
	 "                              >> ");
  scanf("%99s", str);
  
  str_length = strlen(str); // Determine length of string.
  str[str_length] = '\0'; // Add null character to end of string.

  // Shift the value of each character in the string from it's ASCII
  // value by 67 (allows it to be stored as a two digit number).    
  for (size_t i = 0; i < str_length; i++) {
    str[i] -= 65;
  }

  // Store length of string in first two digits of current 'memory' location.
  memory[*operand_ptr] = str_length * 100;
  // Store first string character in last two digits of current 'memory'
  // location.
  memory[*operand_ptr] += str[0];

  // Add the string to Simpletron memory by starting at the current 'memory'
  // position (which is the position following that used to store the length
  // of the string), then moving through 'memory' two digits at a time.
  
  while (str[current] != '\0') {
    str_iter++;
    // Add character to first two digits of 'memory'.
    memory[*operand_ptr + current] = str[str_iter] * 100;
    str_iter++;    
    // Add character to second two digits of 'memory'.
    memory[*operand_ptr + current] += str[str_iter];
    current++;
  }
}

//  *****************************************************************************
//  ***                     Function 'write_string'                           ***
//  *****************************************************************************
void write_string(int memory[MEMORY_SIZE], int *operand_ptr)
{
  unsigned int str_length; // Length of user-entered string.
  unsigned int count = 0; // Marks 'memory' position during string iteration.
  unsigned int str_iter = 0; // Controls string iteration;
  
  // Assign first two digits in current 'memory' location to string length.
  str_length = memory[*operand_ptr] / 100;

  printf("%s%s\n%s", "***                   -String Value-",
  	 "                             ***",
  	 ">> ");
  
  // Read the string from 'memory' by advancing two digits at a time,
  // using the first two digits at the current position in 'memory'
  // to determine the string length.
  while (str_iter < str_length) {
    
    // Don't read the very first two digits as a character - this is
    // the string length.
    if (count == 0) {
      // Read from second two digits in 'memory' location.
      printf("%c", (memory[*operand_ptr] % 100) + 65);
      str_iter++;
    }
    else {
      // Don't print if string length reached.
      if (str_iter != str_length) {
	// Read from first two digits in 'memory' location.
	printf("%c", (memory[*operand_ptr + count] / 100) + 65);
	str_iter++;
      }
      
      // Don't print if string length reached.      
      if (str_iter != str_length) {
	// Read from second two digits in 'memory' location.
	printf("%c", (memory[*operand_ptr + count] % 100) + 65);
	str_iter++;
      }
    }
    count++;
   }
  
  puts(""); 
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
  if (memory[*operand_ptr] == 0X0){
    printf("%s\n%s\n",
	   "XXX                  "
	   "Attempt to divide by zero.                  XXX",
	   "XXX         "
	   " Simpletron execution abnormally terminated.         XXX");
    *terminate_ptr = 0X1;
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
//  ***                       Function 'newline'                              ***
//  *****************************************************************************
void newline(int memory[MEMORY_SIZE], int *operand_ptr)
{
  memory[*operand_ptr] = '\n';
}

//  *****************************************************************************
//  ***                 Function 'branch_instruction'                         ***
//  *****************************************************************************
void branch_instruction(int *instruction_counter_ptr,
			int *operand_ptr, int *toggle_ptr)
{
  *instruction_counter_ptr = *operand_ptr;
  *toggle_ptr = 0X1;
}

//  *****************************************************************************
//  ***                 Function 'branchneg_instruction'                      ***
//  *****************************************************************************
void branchneg_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			   int *operand_ptr, int *toggle_ptr)
{
  if (*accumulator_ptr < 0X0) {
    *instruction_counter_ptr = *operand_ptr;
    *toggle_ptr = 0X1;
  }
}

//  *****************************************************************************
//  ***                Function 'branchzero_instruction'                      ***
//  *****************************************************************************
void branchzero_instruction(int *accumulator_ptr, int *instruction_counter_ptr,
			    int *operand_ptr, int *toggle_ptr)
{
  if (*accumulator_ptr == 0X0) {
    *instruction_counter_ptr = *operand_ptr;
    *toggle_ptr = 0X1;
  }
}

//  *****************************************************************************
//  ***                     Function 'halt_instruction'                       ***
//  *****************************************************************************
void halt_instruction(int *terminate_ptr)
{
  printf("\n%s\n%-18s%20s%19s\n%s\n\n",
	 "********************************************************************",
	 "***",
	 "Simpletron Execution Terminated",
	 "***",
	 "********************************************************************");
  *terminate_ptr = 0X1;
}

//  *****************************************************************************
//  ***                     Function 'increment_counter'                      ***
//  *****************************************************************************
void increment_counter(int *toggle_ptr, int *instruction_counter_ptr)
{
  if (*toggle_ptr == 0X0) {
    int temp = *instruction_counter_ptr;
    temp++;
    *instruction_counter_ptr = temp;
  }
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
  *operation_code_ptr = *instruction_register_ptr / 0X100;
  // Hold operand instruction acts 
  *operand_ptr = *instruction_register_ptr % 0X100; 
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
