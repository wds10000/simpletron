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
  print(4,
	"* ",
	"Enter Hexadecimal Integer",
	" *",
	">> ");
  scanf("%X", &memory[*operand_ptr]);
}

//  *****************************************************************************
//  ***                     Function 'write_instruction'                      ***
//  *****************************************************************************
void write_instruction(int memory[MEMORY_SIZE], int *operand_ptr)
{
  print(4,
	"* ",
	"Value",
	" *",
	">> ");
  printf("%X\n", memory[*operand_ptr]);
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
  print(4,
	"* ",
	"Enter String",
	" *",
	">> ");
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

  print(4,
	"* ",
	"String Value",
	" *",
	">>");
  
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
    print(6,
	  "* ",
	  "Attempt to Divide by Zero",
	  "Simpletron Execution Abnormally Terminated",
	  " *");
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
  print(3,
	"* ",
	"Simpletron Execution Terminated",
	" *");
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

//  *****************************************************************************
//  ***                         Function 'print'                              ***
//  *****************************************************************************
void print(int lines, ...)
{
  // Function uses variable-length argument lists in order to pass an arbitrary
  // number of strings to be printed. Function will print information (info),
  // a promp, appropriately formated, with special cases for printing info
  // headers and footers.
  
  unsigned int length = 0; // Length of string being processed.
  va_list ap; // Initialises object to hold information for function processing
              // as is appropriate for a variable-length argument list.

  va_start(ap, lines); // Initialises object 'ap'.

  // Iterate through each of the lines of text passed to the function, printing
  // them in an appropriate format.
  for (unsigned int i = 1; i <= lines; i++ ) {
    char *passed_string = va_arg(ap, char*); // String passed to function.
    length = strlen(passed_string); // Assign length of current string.
    unsigned int a = (PRINTED - length) / 2; // 'Outside' string portion length.

    // Print appropriate header lines if passed string is the start of info.
    if (!strcmp(passed_string, "* ")) {
      printf("%c", '*');
      for (unsigned int j = 0; j <= PRINTED; j++) {
	printf("%c", '*');
      }
      printf("\n%c", '*');
      
      for (unsigned int j = 0; j < PRINTED; j++) {
	printf("%c", ' ');
      }
      printf("%c\n", '*');
    }
    // Print appropriate footer lines if passed string is the end of info.
    else  if (!strcmp(passed_string, " *")) {
      printf("%c", '*');
      
      for (unsigned int j = 0; j < PRINTED; j++) {
	printf("%c", ' ');
      }
      printf("%c\n", '*');
      printf("%c", '*');
      for (unsigned int j = 0; j <= PRINTED; j++) {
	printf("%c", '*');
      }
      puts("");
    }
    // Print body of info or prompt as appropriate.
    else {
      // Print appropriate character to start line ('*' if info, ' ' if prompt).
      if (strcmp(passed_string, ">> ")) {
	printf("%c", '*'); // Print '*' character to start line.
      }
      else {
	printf("%c", ' '); // Print ' ' character to start line.
      }	

      // Print 'a' spaces preceding the passed string.
      for (unsigned int j = 0; j < a; j++) {
	printf("%c", ' ');
      }

      // Print the passed string.
      printf("%s", passed_string);      

      // Add one to spaces printed if the length of the passed string is odd.
      if (length % 2 != 0) {
	a = (PRINTED - length) / 2 + 1;
      }
    
      // Print 'a' spaces succeding the passed string, or none if prompt passed.
      if (strcmp(passed_string, ">> ")) {
	for (unsigned int j = 0; j < a; j++) {
	  printf("%c", ' ');
	}
      }

      // Print appropriate character to end line ('*' if info, none if prompt).
      if (strcmp(passed_string, ">> ")) {
	printf("%c\n", '*'); // Print '*' character to end line.
      }
    }
  }

  va_end(ap); // Invoke va_end to faciliate normal return from function.
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
