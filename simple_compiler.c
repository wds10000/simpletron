//  *****************************************************************************
//  *
//  *    simple_compiler.c -- Source file for the compiler functions called by
//  *                         'compiler.c'.
//  *    Date Created: Wed Aug 26 10:02:08 2020
//  *
//  *****************************************************************************

#include "simple_compiler.h"

const unsigned int REMARK = 0;
const unsigned int CONST = 1;
const unsigned int ZERO = 2;
const unsigned int VARIABLE = 3;
const unsigned int LINE = 4;
const unsigned int INPUT = 5;
const unsigned int PRINT = 6;
const unsigned int LET = 7;
const unsigned int IF = 8;
const unsigned int READ = 10;
const unsigned int WRITE = 11;
const unsigned int LESS_THAN = 12;
const unsigned int LESS_EQUAL = 13;
const unsigned int EQUAL = 14;
const unsigned int GREATER_EQUAL = 15;
const unsigned int GREATER_THAN = 16;
const unsigned int GOTO = 17;
const unsigned int END = 18;
const unsigned int LOAD = 20;
const unsigned int STORE = 21;
const unsigned int ADD = 30;
const unsigned int SUBTRACT = 31;
const unsigned int MULTIPLY = 32;
const unsigned int DIVIDE = 33;
const unsigned int REMAINDER = 34;
const unsigned int EXPONENT = 35;
const unsigned int BRANCH = 40;
const unsigned int BRANCHNEG = 41;
const unsigned int BRANCHZERO = 42;
const unsigned int HALT = 43;

// ******************************************************************************
// *                             Function 'compile'                             *
// ******************************************************************************
void compile(char simple_file[])
{
  // Define variables.
  char *compiler_memory[COMPILER_MEM_SIZE]; // Holds loaded 'simple' statements.
  Symbols symbol_table[SYMBOL_SIZE]; // Holds symbol data during compilation.
  int flag[SML_MEM_SIZE]; // Holds flags to unresolved references.
  unsigned int SML_memory[SML_MEM_SIZE]; // Holds compiled SML instructions.
  unsigned int instruction_counter = 0; // Number of SML instructions in memory.
  unsigned int *instruction_counter_ptr; // Poiinter to 'instruction_counter'.

  instruction_counter_ptr = &instruction_counter;
  // ****** Sort these out *****
  unsigned int data_counter = 0;
  // ***************************
  
  // Initialise the symbol table and flag array.
  initialise(compiler_memory, SML_memory, symbol_table, flag);
  // Load a 'simple' program into compiler memory.
  load_program(compiler_memory, simple_file);
  // Perform first pass on 'simple' statements saved in 'compiler_memory'.
  first_pass(SML_memory, compiler_memory, symbol_table, flag,
	     instruction_counter_ptr);
  // Perform second pass on 'simple' statements saved in 'compiler_memory'.
  second_pass(SML_memory, compiler_memory, symbol_table, flag, simple_file);
}

// ******************************************************************************
// *                             Function 'initialise'                          *
// ******************************************************************************
void initialise(char *compiler_memory[], unsigned int SML_memory[],
		Symbols symbol_table[],	int flag[])
{
  // Iterate through & initialise 'symbol_table'.
  for (size_t i = 0; i < SYMBOL_SIZE; i++){
    symbol_table[i].symbol = 00;
    symbol_table[i].type = '\0';
    symbol_table[i].location = 00;
    symbol_table[i].is_empty = true;
  }

  // Iterate through & initialise 'SML_memory' and 'flag'.
  for (size_t i = 0; i < SML_MEM_SIZE; i++){
    SML_memory[i] = 0;

    flag[i] = -1;
  }
  
  // Iterate through 'compiler_memory' and initialise each string with calloc.
  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
    compiler_memory[i] = calloc(1, sizeof(char));
    strcpy(compiler_memory[i], "(empty)"); // Assign '(empty)' to strings.
  }
}

// ******************************************************************************
// *                             Function 'load_program'                        *
// ******************************************************************************
void load_program(char *compiler_memory[], char simple_file[])
{
  FILE *file_ptr; // Declare new file pointer
  size_t i = 0; // Increments to the next element of 'compiler_memory'.
  size_t j = 0; // Increments to the next character in 'compiler_memory[i]'.

  // If file was successfully loaded, assign each 'simple' statement contained
  // therein to 'compiler_memory[i]'.
  if ((file_ptr = fopen(simple_file, "r+")) != NULL) {

    // Until end-of-file is reached, scan each character from the file.
    while (!feof(file_ptr)) {
      fscanf(file_ptr, "%c", &compiler_memory[i][j]);

      // If newline, replace with null character & increment 'compiler_memory'.
      if (compiler_memory[i][j] == '\n') {
	compiler_memory[i][j] = '\0';
	i++;
	j = 0;
      }
      // If not newline, increment to next character in 'compiler_memory[i]'
      else {
	j++;
      }
      // Resize array 'compiler_memory[i]'.
      compiler_memory[i] = realloc(compiler_memory[i], (j + 1) * sizeof(char));
    }
    fclose(file_ptr);
  }
  // If file was not successfully loaded, print an error message.
  else {
    printf("%s\n", "File was not loaded successfully.");
  }
}

// ******************************************************************************
// *                             Function 'first_pass'                          *
// ******************************************************************************
void first_pass(unsigned int SML_memory[], char *compiler_memory[],
		Symbols symbol_table[], int flag[],
		unsigned int *instruction_counter_ptr)
{
  char **tokenised_string; // 'simple' statement to be tokenised.
  char **temp_string; // Holds address of 'tokenised_string' temporarily.
  char *infix; // Holds an assignment statement in infix form.
  char *postfix; // Holds an assignment statement in postfix form.
  unsigned int token_number; // Number of tokens in tokenised string.
  unsigned int token_value; // Indicates return value for 'compare_token'.
  unsigned int table_index = 0; // Position of element in symbol table.
  unsigned int token_position; // Position of token in string.
  int memory_location = 0; // Indicates a token has been repeated.  
  unsigned int repeat_value = 0; // Indicates a token has been repeated.
  unsigned int symbol_counter = 0; // Tracks number of constants and variables.
  unsigned int previous_counter; // Holds the last value of '*instruction_counter_ptr'.
  bool let; // Indicates whether the current 'simple' statement is an assignment.
  bool branch; // Indicates whether the current 'simple' statement is
               // a conditional statement.
  bool go_to; // Indicates whether the current 'simple' statement is a
              // conditional statement or a 'GOTO' statement.
  bool less_equal; // Indicates whether 'token_value' == 'LESS_EQUAL'.
  bool greater_equal; // Indicates whether 'token_value' == 'GREATER_EQUAL'.
  int op1; // First operator of equality operator.
  int op2; // Second operator of equality operator.
  unsigned int temp; // Holds equality operator operand while swapping.

  unsigned int *table_index_ptr; // Pointer to 'table_index'.
  unsigned int *repeat_value_ptr; // Pointer to 'repeat_value'.
  unsigned int *symbol_counter_ptr; // Pointer to 'symbol_counter'.
  int *memory_location_ptr; // Pointer to 'memory_location'.
  table_index_ptr = &table_index;
  repeat_value_ptr = &repeat_value;
  symbol_counter_ptr = &symbol_counter;
  memory_location_ptr = &memory_location;  

  // ***************** vv COMMENT OUT TO SUPRESS PRINTING vv *******************

  /* printf("\n\t\t\t\t%s\n\n\t\t\t\t%s\n\t\t\t\t%2c\t%2c\t%2c\n\t\t\t\t%s\n\n", */
  /* 	 "    Symbol Table", */
  /* 	 "*******************", */
  /* 	 'S', 'T', 'L', */
  /* 	 "*******************"); */

  // ***************** ^^ COMMENT OUT TO SUPRESS PRINTING ^^ *******************

  // Iterate through the 'simple' statements in compiler memory & tokenise each.
  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
    infix = calloc(1, sizeof(char));
    postfix = calloc(1, sizeof(char));
    token_number = 0;
    token_position = 0; // Reset token position.
    // Set initial values for conditional/GOTO variables.
    let = false;
    branch = false;
    go_to = false;
    less_equal = false;
    greater_equal = false;
    op1 = -1;
    op2 = -1;
    
    // If 'compiler_memory' is empty, exit the for loop.
    if (!strcmp(compiler_memory[i], "(empty)")) {
      break;
    }
    
    // Tokenise 'compiler_memory[i]'
    tokenised_string = tokenise(tokenised_string, compiler_memory[i]);

    // Determine number of tokens in the tokenised string.
    temp_string = tokenised_string;

    // Iterate through 'temp_string', counting the number of tokens.
    while (*temp_string != NULL) {
      token_number++; // Increment the number of tokens in the tokenised string.
      temp_string++;
    }
    
    // Compare each token in the passed string to the various 'simple' keywords,
    // variables and constant in order to decide how to process the token.
    while (*tokenised_string != NULL) {
      // Assign 'token_value' according to the nature of the current token.
      token_value = compare_token(*tokenised_string, token_position, SML_memory,
				  compiler_memory, symbol_table, flag,
				  &memory_location_ptr, repeat_value_ptr,
				  table_index_ptr, go_to);

      // ****************************** REMARK *********************************
      
      // If line is a remark, exit the loop.
      if (token_value == REMARK) {
      	break;
      }

      // ********************* REPEATED CONSTANT/VARIABLE **********************
      
      // If the token is a repeated constant or variable, assign
      // 'repeat_value_ptr' to 'table_index_ptr'.
      if (token_value == -CONST || token_value == -ZERO || token_value == -VARIABLE) {
	*table_index_ptr = *repeat_value_ptr;
      }
 
      // *********************** POPULATE SYMBOL TABLE *************************
      
      // Assign line numbers, constant and variables to 'symbol_table'.
      if (token_value == CONST) { // Assign constant to 'symbol_table'.
      	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr, go_to);
      }
      if (token_value == ZERO) { // Assign constant (zero) to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr, go_to);
      }
      if (token_value == VARIABLE) { // Assign variable to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr, go_to);
      }
      if (token_value == LINE) { // Assign line number to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr, go_to);
      }

      // *************************** LOAD CONSTANT *****************************
      
      // If token is a constant, assign to SML memory.
      if (token_value == CONST || token_value == ZERO) {
	SML_memory[SML_MEM_SIZE - *symbol_counter_ptr - 1]
	  = symbol_table[*table_index_ptr].symbol;
	(*symbol_counter_ptr)++;
      }

      // **************************** LOAD VARIABLE ****************************
      
      // If the token is a variable, assign to SML memory.
      if (token_value == VARIABLE) {
	SML_memory[SML_MEM_SIZE - *symbol_counter_ptr - 1]
	  = symbol_table[*table_index_ptr].symbol;
	(*symbol_counter_ptr)++;	
      }
 
      // *********************** LOAD 'INPUT' INSTRUCTION **********************
      
      // If program asks for input, initialise 'READ' instruction.
      if (token_value == INPUT) {
	SML_memory[*instruction_counter_ptr] = 100 * READ;
      }

      // If token is a variable and succeeds 'input', add location of operand in
      // SML memory to previous 'READ' instruction.
      if ((token_value == -VARIABLE || token_value == VARIABLE) && token_position == 2 &&
	  SML_memory[*instruction_counter_ptr] == 100 * READ) {
	SML_memory[*instruction_counter_ptr] +=
	  symbol_table[*table_index_ptr].location;
	(*instruction_counter_ptr)++;
      }    

      // *********************** LOAD 'PRINT' INSTRUCTION **********************
      
      // If program outputs data, initialise 'WRITE' instruction.
      if (token_value == PRINT) {
	SML_memory[*instruction_counter_ptr] = 100 * WRITE;
      }

      // If token is a variable and succeeds 'output', add location of operand in
      // SML memory to previous 'WRITE' instruction.
      if ((token_value == -VARIABLE || token_value == VARIABLE) && token_position == 2 &&
	  SML_memory[*instruction_counter_ptr] == 100 * WRITE) {
	SML_memory[*instruction_counter_ptr] +=
	  symbol_table[*table_index_ptr].location;
	(*instruction_counter_ptr)++;
      }

      // ************************ LOAD 'LET' INSTRUCTION ***********************

      // If program assigns data to a variable, initialise appropriate
      // instructions.
      if (token_value == LET) {
	let = true;
      }

      // If 'let' == 'true', 
      if (let == true) {
	// If the current token is at the beginning of the right side of the
	// assignment statement, add the remaining tokens to 'infix'.
	if (token_position >= 4) {
	  infix = realloc(infix, (strlen(infix)
				  + strlen(*tokenised_string) + 2) * sizeof(char));
	  strcat(infix, *tokenised_string);
	  strcat(infix, " ");
	  infix[strlen(infix)] = '\0';
	  postfix = realloc(postfix, (strlen(postfix)
				      + strlen(*tokenised_string) + 1) * sizeof(char));
	  strcat(postfix, " ");
	}

	// If the current token is the last in the 'simple' statement,
	// call 'convert_to_postfix' and 'evaluate_postfix' to convert
	// the right side of the assignment to postfix form, then
	// evaluate it.

	// Need to change funcitons to take input if constants are multi digit.
	
	if (token_position == token_number - 1) {
	  convert_to_postfix(infix, postfix);
	  postfix[strlen(postfix)] = '\0';

	  evaluate_postfix_expression(SML_memory, symbol_table, postfix,
	  			      instruction_counter_ptr);
	}
      }

      // ************************ CONDITIONAL STATEMENTS ***********************

      // If the current statement is a conditonal-goto instruction,
      // set 'branch' = true.
      if (token_value == IF) {
    	branch = true;
      }

      // Assign the memory addresses of the operands to 'op1' & 'op2'.
      if (branch == true) {

	// If the current token is a variable.
	if (token_value == VARIABLE || token_value == -VARIABLE) {

	  // Initialise 'LOAD' instruction & assign variable to 'op1'.
	  if (op1 == -1) {
	    // Initialise 'LOAD' instruction & increment '*instruction_counter_ptr'.
	    SML_memory[*instruction_counter_ptr] = 100 * LOAD;
	    (*instruction_counter_ptr)++;
	    // Initialise 'SUBTRACT' instruction & increment '*instruction_counter_ptr'.
	    SML_memory[*instruction_counter_ptr] = 100 * SUBTRACT;
	    (*instruction_counter_ptr)++;

	    // Assign first variable in 'simple' statement to 'op1'.
	    op1 = symbol_table[*table_index_ptr].location;
	  }
	  // Assign first variable in 'simple' statement to 'op1'.
	  else {
	    op2 = symbol_table[*table_index_ptr].location;
	  }
	}

	// Initialise appropriate branch instruction.
	switch (token_value) {

	case 12: // LESS_THAN.
	  // If value in accumulator is less than zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHNEG;
	  break;
	case 13: // LESS_EQUAL.
	  less_equal = true; // Set true if 'token' == 'LESS_EQUAL'.

	  // If value in accumulator is less than zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHNEG;
	  (*instruction_counter_ptr)++;
	  
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHZERO;
	  break;
	case 14: // EQUAL.
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHZERO;
	  break;
	case 15: // GREATER_EQUAL.
	  greater_equal = true; // Set true if 'token' == 'GREATER_EQUAL'.

	  // Swap operands, which turns 'GREATER_EQUAL' into 'LESS_EQUAL'.
	  temp = op1;
	  op1 = op2;
	  op2 = temp;
	    
	  // If value in accumulator is less than zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHNEG;
	  (*instruction_counter_ptr)++;
	  
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHZERO;
	  break;
	case 16: // GREATER_THAN.
	  // Swap operands, which turns 'GREATER_THAN' into 'LESS_THAN'.
	  temp = op1;
	  op1 = op2;
	  op2 = temp;
	    
	  // If value in accumulator is less than zero, branch.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCHNEG;
	  break;
	}
      }
      
      // If 'token_value' == 'GOTO', set 'go_to' == 'true'.
      if (token_value == GOTO) {
	go_to = true;
      }

      // Add operand in SML memory to appropriate branch instruction(s).
      if (go_to == true && token_value == LINE) {

	// If 'branch' == 'true', add operand in SML memory to branch
	// instruction(s) chosen in the switch statement above.
	if (branch == true) {

	  // If the equality operator is '<=' or '>=', add op1, op2 and memory
	  // location of current line number to instructions at '*instruction_counter_ptr - 3',
	  // '*instruction_counter_ptr -2', '*instruction_counter_ptr - 1' and '*instruction_counter_ptr'.
	  if(less_equal == true || greater_equal == true) {
	    // Add 'op1' to the 'LOAD' instruction.
	    SML_memory[*instruction_counter_ptr - 3] += op1;

	    // Add 'op2' to the 'SUBTRACT' instruction.
	    SML_memory[*instruction_counter_ptr - 2] += op2;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string, &memory_location_ptr)) {
	      // Add memory location of current line number to first branch instruction.
	      SML_memory[*instruction_counter_ptr] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[*instruction_counter_ptr] = atoi(*tokenised_string);	     
	    }
	    (*instruction_counter_ptr)++;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string, &memory_location_ptr)) {	    
	      // Add memory location of current line number to second branch instruction.
	      SML_memory[*instruction_counter_ptr] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[*instruction_counter_ptr] = atoi(*tokenised_string);	     
	    }
	    (*instruction_counter_ptr)++;
	  }
	  else {
	    // Add 'op1' to the 'LOAD' instruction.
	    SML_memory[*instruction_counter_ptr - 2] += op1;

	    // Add 'op2' to the 'SUBTRACT' instruction.
	    SML_memory[*instruction_counter_ptr - 1] += op2;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string, &memory_location_ptr)) {	    
	      SML_memory[*instruction_counter_ptr] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[*instruction_counter_ptr] = atoi(*tokenised_string);	     
	    }
	    (*instruction_counter_ptr)++;	    
	  }
	}
	else {
	  // Initialise 'BRANCH' instruction, then add operand in SML memory.
	  SML_memory[*instruction_counter_ptr] = 100 * BRANCH;

	  // If the line number after 'goto' is present in the symbol table, add
	  // the SML memory location referred to by the line 'location' to 'BRANCH'
	  // instruction. Else, add the line number to the element of 'flag' with the
	  // same index as 'symbol_table'.
	  if (search_table(symbol_table, LINE, *tokenised_string, &memory_location_ptr)) {	  
	    SML_memory[*instruction_counter_ptr] += symbol_table[*repeat_value_ptr].location;
	  }
	  // If the line number after 'goto' references a line ahead of the
	  // current position, add the line number to the element 'flag'
	  // corresponding to the same element in 'SML_memory'.
	  else {
	    flag[*instruction_counter_ptr] = atoi(*tokenised_string);	     
	  }
	  (*instruction_counter_ptr)++;
	}	
      }

      // *************************** END STATEMENT *****************************

      // If program terminates, assign 'HALT' instruction to SML memory.
      if (token_value == END) {
	SML_memory[*instruction_counter_ptr] = 100 * HALT;
      }

      // ************************* INCREMENT VARIABLES *************************
      
      // Increment to the next token.
      tokenised_string++;
      token_position++;
    }
  }
}

// ******************************************************************************
// *                             Function 'second_pass'                         *
// ******************************************************************************
void second_pass(unsigned int SML_memory[], char *compiler_memory[],
		 Symbols symbol_table[], int flag[], char simple_file[])
{
  // Replace unresolved references in SML memory.
  replace_references(flag, SML_memory, symbol_table);
  
  // Save compiled 'SML' code to disk.
  save_file(SML_memory, simple_file);
  
  // ***************** vv COMMENT OUT TO SUPRESS PRINTING vv *******************

  /* printf("\n\t\t\t\t%s\n\n", "    Memory Dump"); */

  /* for (unsigned int i = 0; i < 86; i++) { */
  /*   printf("%s", "*"); */
  /* } */
  /* printf("\n%c", '\t'); */

  /* for (unsigned int i = 0; i < 10; i++) { */
  /*   printf("%6d\t", i); */
  /* } */
  /* puts(""); */

  /* for (unsigned int i = 0; i < 86; i++) { */
  /*   printf("%s", "*"); */
  /* } */
  /* puts(""); */

  /* /\* // SML memory. *\/ */
  /* for (size_t i = 0; i < SML_MEM_SIZE; i++) { */

  /*   if (i % 10 == 0) { */
  /*     printf("  %ld%s", i / 10, " | "); */
  /*   } */

  /*   if (SML_memory[i] >= 97 && SML_memory[i] <= 122) { */
  /*     printf("\t %5c", SML_memory[i]); */
  /*   } */
  /*   else { */
  /*     printf("\t +%04d", SML_memory[i]); */
  /*   } */
  /*   if ((i + 1) % 10 == 0) { */
  /*     puts(""); */
  /*   } */
  /* } */

  /* //  Flag array. */
  /* for (size_t i = 0; i < SML_MEM_SIZE; i++) { */

  /*   printf("%d ", flag[i]); */
    
  /*   if ((i + 1) % 10 == 0) { */
  /*     puts(""); */
  /*   } */
  /* } */

  // Symbol table.
  /* for (size_t i = 0; i < SML_MEM_SIZE; i++) { */
  /*   if (symbol_table[i].symbol >= 97 && symbol_table[i].symbol <= 122) { */
  /*     printf("\t %c", symbol_table[i].symbol); */
  /*   } */
  /*   else { */
  /*     printf("\t %d", symbol_table[i].symbol); */
  /*   } */
  /*   if ((i + 1) % 10 == 0) { */
  /*     puts(""); */
  /*   } */
  /* } */
  /* puts(""); */

  // ***************** ^^ COMMENT OUT TO SUPRESS PRINTING ^^ *******************
}

// ******************************************************************************
// *                             Function 'tokenise'                            *
// ******************************************************************************
char** tokenise(char **tokenised_string, char *simple_statement)
{
  // Assign 'tokenised_string' to 'temp_string' for function processing.
  char **temp_string = tokenised_string;
  unsigned int tokens = 0; // Current number of processed tokens.

  // Tokenise the first segment of the passed string
  char *token_ptr = strtok(simple_statement, " ");

  // Allocate memory to 'temp_string' using calloc.
  temp_string = calloc(tokens + 1, sizeof(char*));

  // While there are more tokens, keep parsing the passed string and assign each
  // token to the elements of 'temp_string'.
  while (token_ptr != NULL){
    temp_string[tokens] = token_ptr;
    tokens++;
    temp_string = realloc(temp_string, (tokens + 1) * sizeof(char*));
    token_ptr = strtok(NULL, " ");
  }

  // Set the last element in 'temp_string' equal to NULL.
  temp_string[tokens] = NULL;

  // Use calloc to change the size of 'tokenised_string' to 'tokens + 1'.
  tokenised_string = calloc(tokens + 1, sizeof(char*));

  // Assign the address of 'temp_string' to 'tokenised_string' and return.
  tokenised_string = &temp_string[0];

  return tokenised_string;
}

// ******************************************************************************
// *                             Function 'compare_token'                       *
// ******************************************************************************
unsigned int compare_token(char *token, unsigned int token_position,
			   unsigned int SML_memory[], char *compiler_memory[],
			   Symbols symbol_table[], int flag[],
			   int **memory_location_ptr,
			   unsigned int *repeat_value_ptr,
			   unsigned int *table_index_ptr, bool go_to)

{
  // Compare the passed token to the various 'simple' keywords, variables and
  // constants and call the appropriate function.

  if (!strcmp(token, "rem")) { // If the token is part of a remark.
    return REMARK;  
  }
  else if (token_position != 0) { // If token is a constant.
    if (go_to == false) { // If token is not a line number following 'goto'.
      if (atoi(token)) { // If the token is a number other than 0.
	// Search table for 'token'.
	if (!(*repeat_value_ptr = search_table(symbol_table, CONST, token,
					       memory_location_ptr))) {
	  // If present, return CONST.
	  return CONST;
	}
	else {
	  return -CONST; // If symbol present in table, return -1.
	}
      }
      if (!strcmp(token, "0")) { // If the token is 0.
	// Search table for 'token'.
	if (!(*repeat_value_ptr = search_table(symbol_table, ZERO, token,
					       memory_location_ptr))) {
	  // If present, return ZERO.
	  return ZERO;
	}
	else {
	  return -ZERO; // If symbol present in table, return -1.
	}
      }
    }
    else {
      *repeat_value_ptr = search_table(symbol_table, LINE, token,
				       memory_location_ptr);
      return LINE;
    }
  }
  if (strlen(token) == 1) { // If token is a variable (ie. alphabetic character).
    if (*token >= 97 && *token <= 122) {
      // Search table for 'token'.
      if (!(*repeat_value_ptr = search_table(symbol_table, VARIABLE, token,
					     memory_location_ptr))) {
	// If present, return VARIABLE.
	return VARIABLE;
      }
      else {
	return -VARIABLE; // If symbol present in table, return -1.
      }
    }
  }
  if (token_position == 0) { // If token is a line number.
    // Search table for 'token'.
    if (!(*repeat_value_ptr = search_table(symbol_table, LINE, token,
					   memory_location_ptr))) {
      // If present, return LINE.
      return LINE;
    }
    else {
      return -LINE; // If symbol present in table, return -1.
    }
  }
  
  // The following selection statements compare 'token' to 'simple' keywords
  // and equality operators. Use dummy variables 'x' and 'y' for demonstration

  if (!strcmp(token, "input")) { // If user asked for input.
    return INPUT;
  }
  if (!strcmp(token, "print")) { // If program writes output.
    return PRINT;
  }
  if (!strcmp(token, "let")) { // If program assigns to a variable.
    return LET;
  }
  if (!strcmp(token, "if")) { // Start of a branch statement.
    return IF;
  }
  if (!strcmp(token, "<")) { // If x - y < 0.
    return LESS_THAN;
  }
  if (!strcmp(token, "<=")) { // If x - y <= 0.
    return LESS_EQUAL;
  }
  if (!strcmp(token, "==")) { // If x - y = 0.
    return EQUAL;
  }
  if (!strcmp(token, ">=")) { // If x - y >= 0.
    return GREATER_EQUAL;
  }
  if (!strcmp(token, ">")) { // If x - y > 0.
    return GREATER_THAN;
  }
  if (!strcmp(token, "goto")) { // Program branches to another line.
    return GOTO;
  }
  if (!strcmp(token, "end")) { // Program ends execution.
    return END;
  }
}

// ******************************************************************************
// *                             Function 'search_table'                        *
// ******************************************************************************
unsigned int search_table(Symbols symbol_table[], unsigned int symbol_code,
			  char *token, int **memory_location_ptr)
{
  unsigned int count = 0; // Controls search iteration.

  // Iterate through the symbol table. If the symbol is present, return 0. If
  // it is not present, retun count.
  while (count < SYMBOL_SIZE) {

    switch (symbol_code) {
      
    case 1: // If the symbol is a constant.
      if (symbol_table[count].type == 'C') {
	if (atoi(token) == symbol_table[count].symbol) {
	  *memory_location_ptr = &symbol_table[count].location;	  
	  return count;
	}
      }
      break;
    case 2: // If the symbol is a constant (0).
      if (0 == symbol_table[count].symbol && 'C' == symbol_table[count].type) {
	*memory_location_ptr = &symbol_table[count].location;
	return count;
      }
      break;
    case 3: // If the symbol is a variable (alphabetic character constant).
      if (token[0] == symbol_table[count].symbol) {
	*memory_location_ptr = &symbol_table[count].location;
	return count;
      }
      break;
    case 4: // If the symbol is a line number.
      if (symbol_table[count].type == 'L') {
	if (atoi(token) == symbol_table[count].symbol) {
	  *memory_location_ptr = &symbol_table[count].location;
	  return count;
	}
      }
      break;
    }
    count++;
  }

  return 0;
}

// ******************************************************************************
// *                             Function 'add_symbol'                          *
// ******************************************************************************
unsigned int add_symbol(unsigned int token_value, char *token,
			Symbols symbol_table[], unsigned int *symbol_counter_ptr,
			bool go_to)
{
  unsigned int count; // Variable iterates through 'symbol_table'.

  // Iterate through the table until an empty element is found.
  for (count = 0; count < SYMBOL_SIZE; count++) {

    if (symbol_table[count].is_empty) {	

      if (token_value == CONST) { // If the token is a constant.
	symbol_table[count].symbol = atoi(token); // Assign constant to element. 
	symbol_table[count].type = 'C'; 
	symbol_table[count].location = SML_MEM_SIZE - *symbol_counter_ptr - 1;
	symbol_table[count].is_empty = false; // Element is occupied.
	break;
      }

      if (token_value == ZERO) { // If the token is zero.
	symbol_table[count].symbol = 0; // Assign zero to element.
	symbol_table[count].type = 'C'; 
	symbol_table[count].location = SML_MEM_SIZE - *symbol_counter_ptr - 1;
	symbol_table[count].is_empty = false; // Element is occupied.
	break;
      }	  

      if (token_value == VARIABLE) {	// If the token is a variable.
	symbol_table[count].symbol = token[0]; 
	symbol_table[count].type = 'V'; 
	symbol_table[count].location = SML_MEM_SIZE - *symbol_counter_ptr - 1;
	symbol_table[count].is_empty = false; // Element is occupied.
	break;
      }
	
      if (token_value == LINE) { // If the token is a line number.

	if (go_to == false) { // Don't add if line follows 'GOTO' keyword.

	  if (!strcmp(token, "00")){ // If line number is '00'.
	    symbol_table[count].symbol = 00; // Assign variable to this element.
	  }
	  
	  else { // Otherwise, use atoi to assign token to element.
	    symbol_table[count].symbol = atoi(token); // Assign variable to element.
	  }
	  symbol_table[count].type = 'L'; 
	  symbol_table[count].location = count - *symbol_counter_ptr;
	  symbol_table[count].is_empty = false; // Element is occupied.
	  break;
	}
	else {
	  return count;
	}
      }
    }
  }

  // ***************** vv COMMENT OUT TO SUPRESS PRINTING vv *******************

  /* if (token_value == VARIABLE) { */
  /*   printf("\t\t\t\t%2c\t%2c\t%02d\n", (char) symbol_table[count].symbol, symbol_table[count].type, */
  /* 	   symbol_table[count].location); */
    
  /* } */
  /* else if (symbol_table[count].symbol != '0') { */
  /*   printf("\t\t\t\t%2d\t%2c\t%02d\n", symbol_table[count].symbol, symbol_table[count].type, */
  /* 	   symbol_table[count].location); */
  /* } */
  /* else { */
  /*   printf("\t\t\t\t%2c\t%2c\t%02d\n", symbol_table[count].symbol, symbol_table[count].type, */
  /* 	   symbol_table[count].location); */
  /* } */

  // ***************** ^^ COMMENT OUT TO SUPRESS PRINTING ^^ *******************

  return count;
}

// ******************************************************************************
// *                         Function 'replace_references'                      *
// ******************************************************************************
void replace_references(int flag[], unsigned int SML_memory[],
			Symbols symbol_table[])
{
  // Iterate through 'flag' and find any unresolved references (flag[i] != -1).
  for (size_t i = 0; i < SML_MEM_SIZE; i++) {

    // If an unresolved reference is found, iterate through 'symbol_table' until a symbol
    // with a location matching the unresolved reference is found.
    if (flag[i] != -1) {

      for (size_t j = 0; j < SYMBOL_SIZE; j++) {

	// If a match is found (and the symbol type is 'L'), add the location
	// stored in the symbol's location to the instruction in 'SML_memory' with
	// the same element number as the unresolved refernce in 'flag'.
	if (symbol_table[j].symbol == flag[i] && symbol_table[j].type == 'L') {

	  SML_memory[i] += symbol_table[j].location;
	}	
      }
    }
  }
}

// ******************************************************************************
// *                             Function 'save_file'                           *
// ******************************************************************************
void save_file(unsigned int SML_memory[], char simple_file[])
{
  FILE *file_ptr; // Declare new file pointer.

  // Remove the '.txt' suffix. from 'simple_file' and append 'file_suffix'.
  // **********************************************************************
  int j = 0;
  char file_suffix[] = "_(SML_code).txt";
  unsigned int length1 = strlen(file_suffix);
  unsigned int length2 = strlen(simple_file);
  char SML_instructions[length1 + length2];
  
  for (size_t i = 0; i < length1 + length2; i++) {
    
    if (i < length1) {
      SML_instructions[i] = simple_file[i];
    }
    else {
      SML_instructions[i] = file_suffix[j];
      j++;
    }
  }
  // **********************************************************************
  
  // If file was successfully loaded, print each SML instruction statement in
  // 'SML_memory' to file.
  if ((file_ptr = fopen(SML_instructions, "w+")) != NULL) {
    for (size_t i = 0; i < SML_MEM_SIZE; i++) {
      fprintf(file_ptr, "%d\n", SML_memory[i]);
    }
    fclose(file_ptr); // Close current file.
  }
  // If file was not successfully loaded, print an error message.
  else {
    printf("%s\n", "Output file was not saved successfully.");
  }
}

// ******************************************************************************
// *                             Function 'convert_to_postfix'                  *
// ******************************************************************************
void convert_to_postfix(char infix[], char postfix[])
{
  Stack_Node_Ptr top_node_ptr; // Points to the top of the stack.
  top_node_ptr = NULL; // Initialise 'top_node_ptr'.
  unsigned int i = 0; // Current element index in 'infix'.
  unsigned int p = 0; // Current element index in 'postfix'.
  char a; // Current character in 'infix'.

  push(&top_node_ptr, '('); // Push '(' onto the stack.

  infix[strlen(infix)] = ')'; // Append ')' to 'infix'.
  infix[strlen(infix) + 1] = '\0';

  // While the stack is not empty, read 'infix' from left to right.
  while (!is_empty(top_node_ptr)) {
    a =  infix[i];
 
    // If the current character is ' ', ignore it.
    if (isspace(a)) {
      ;
    }
    // If the current character in 'infix' is a digit, copy it to the next
    // element of 'postfix'.
    else if (isalpha(a)) {
      postfix[p] = a;
      p++;
    }
    // If the current character in 'infix' is an alphabetical character,
    // copy it to the next element of 'postfix'.
    else if (isdigit(a)) {
      postfix[p] = a;
      p++;
    }
    // If the current character in 'infix' is '(', push it onto the stack.
    else if (a == '(') {
      push(&top_node_ptr, a);
    }
    // If the current character in 'infix' is an operator,
    else if (is_operator(a)) {
      // Pop operator at the top of the stack while they have equal or
      // higher precedence than the current operator, and insert the
      // popped operators in 'postfix'.
      if (is_operator(stack_top(top_node_ptr))) {
	if (precedence(a, (top_node_ptr)->data) < 0) {
	  postfix[p] = pop(&top_node_ptr);
	  p++;
	}
      }
      // Push the current character in 'infix' onto the stack.
      push(&top_node_ptr, a);
    }
    // If the current character in 'infix' is ')',
    else if (a == ')') {
 
      // While the character at the top of the stack is not '(',
      // pop operators from the topc of the stack and insert them
      // in 'postfix'.
      while (stack_top(top_node_ptr) != '(') {
	postfix[p] = pop(&top_node_ptr);
	p++;
      }
      // Pop and discard '(' from the stack.
      pop(&top_node_ptr);
    }
    i++;
  }

  // Append '\0' to the end of 'postfix' and print.
  infix[strlen(infix) - 1] = '\0';
  postfix[p] = '\0';
}

// ******************************************************************************
// *                 Function 'evaluate_postfix_expression'                     *
// ******************************************************************************
int evaluate_postfix_expression(unsigned int SML_memory[], Symbols symbol_table[],
				char postfix[], unsigned int *instruction_counter_ptr)
{
  Stack_Node_Ptr top_node_ptr; // Points to the top of the stack.
  top_node_ptr = NULL; // Initialise 'top_node_ptr'.
  unsigned int i = 0; // Loop counter while iterating through 'symbol_table'.
  unsigned int x, y; // Operand variables.
  int memory_location = 0; // Indicates a token has been repeated.
  int *memory_location_ptr; // Pointer to 'memory_location'.
  memory_location_ptr = &memory_location;
  
  char str[2]; // Holds the value of the current character in 'postfix'.
  
  // While '\0' has not been encountered, read 'expression' from left to right.
  while (postfix[i] != '\0'){
    str[0] = postfix[i];
    str[1] = '\0';
    
    /* printf("%s%d\n", "IC: ", *instruction_counter_ptr); */

    // If the current character in 'postfix' is not an operator, push the 
    // associated memory location (*memory_location_ptr.location) onto the stack.

    // If the current character in 'postfix' is a constant (non zero).
    if (search_table(symbol_table, CONST, str, &memory_location_ptr)) {
      push(&top_node_ptr, *memory_location_ptr);
    }
    // If the current character in 'postfix' is a constant (zero).
    else if (search_table(symbol_table, ZERO, str, &memory_location_ptr)) {
      push(&top_node_ptr, *memory_location_ptr);
    }
    // If the current character in 'postfix' is a variable.
    else if (search_table(symbol_table, VARIABLE, str, &memory_location_ptr)) {
      push(&top_node_ptr, *memory_location_ptr);
    }
    // If the current character is an operator, pop the top two elements of the
    // stack and assign them to variables x and y. Call 'load_instructions' to
    // initialise and load the appropriate instructions into SML memory.
    else {
      x = pop(&top_node_ptr);
      y = pop(&top_node_ptr);

      load_instruction(SML_memory, x, y, postfix[i], instruction_counter_ptr);
    }
    i++;
  }
  // Print the result of the 'postfix' evaluation.
}

// ******************************************************************************
// *                         Function 'load_instruction'                        *
// ******************************************************************************
void load_instruction(unsigned int SML_memory[], int operand1,
		      int operand2, char operator,
		      unsigned int *instruction_counter_ptr)
{
  // Initialise 'LOAD' instruction.
  SML_memory[*instruction_counter_ptr] = 100 * LOAD;
  // Add 'operand2' to the 'LOAD' instruction.
  SML_memory[*instruction_counter_ptr] += operand2;
  (*instruction_counter_ptr)++; // Increment '*instruction_counter_ptr'.

  // Determine which type of instruction to initialise based on the value of the
  // operator, then add 'operand2' to that instruction.
  switch (operator) {
    
  case '+':  // Initialise 'ADD' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * ADD;
    SML_memory[*instruction_counter_ptr] += operand1;
    break;
  case '-': // Initialise 'SUBTRACT' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * SUBTRACT;
    SML_memory[*instruction_counter_ptr] += operand1;
    break;
  case '*':  // Initialise 'MULTIPLY' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * MULTIPLY;
    SML_memory[*instruction_counter_ptr] += operand1;
   break;
  case '/': // Initialise 'DIVIDE' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * DIVIDE;
    SML_memory[*instruction_counter_ptr] += operand1;
    break;
  case '%': // Initialise 'REMAINDER' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * REMAINDER;
    SML_memory[*instruction_counter_ptr] += operand1;
    break;
  case '^': // Initialise 'EXPONENT' instruction.
    SML_memory[*instruction_counter_ptr] = 100 * EXPONENT;
    SML_memory[*instruction_counter_ptr] += operand1;
    break;
  default:
    break;
  }
  (*instruction_counter_ptr)++;
  
  // Store 'operand2' in SML memory.
  SML_memory[*instruction_counter_ptr] = 100 * STORE;
  SML_memory[*instruction_counter_ptr] += operand2;
  (*instruction_counter_ptr)++;
}

// ******************************************************************************
// *                             Function 'is_operator'                         *
// ******************************************************************************
int is_operator(char c)
{
  // Compare 'c' to each of the relevant operators. If there is a match,
  // return '1', otherwise return 0.
  switch (c) {

  case '+': // Addition.
    return 1;
    break;
  case '-': // Subtraction.
    return 1;
    break;
  case '*': // Multiplication.
    return 1;
    break;
  case '/': // Division.
    return 1;
    break;
  case '^': // Exponentiation.
    return 1;
    break;
  case '%': // Remainder.
    return 1;
    break;
  default: // Not an operator.
    return 0;
    break;
  }
}

// ******************************************************************************
// *                             Function 'precedence'                          *
// ******************************************************************************
int precedence(char operator1, char operator2)
{
  int a = 0; // Precedence value of operator1;
  int b = 0; // Precedence value of operator2;

  // Set precedence value = 0 if operator is '+' or '-'.
  if ((operator1 == '+') || (operator1 == '-')) {
    a = 0;      
  }
  if ((operator2 == '+') || (operator2 == '-')) {
    b = 0;      
  }
  // Set precedence value = 1 if operator is '*', '/' or '%'.
  if ((operator1 == '/') || (operator1 == '*') || (operator1 == '%')) {
    a = 1;      
  }
  if ((operator2 == '/') || (operator2 == '*') || (operator2 == '%')) {
    b = 1;      
  }
  // Set precedence value = 2 if operator is '^'.
  if (operator1 == '^') {
    a = 2;
  }
  if (operator2 == '^') {
    b = 2;
  }

  // Compare precedence values and return accordingly.
  if (a < b) {
    return -1;
  }
  else if (a == b) {
    return 0;
  }
  else {
    return 1;
  }
}

// ******************************************************************************
// *                             Function 'push'                                *
// ******************************************************************************
void push(Stack_Node_Ptr *top_ptr, char value)
{
  Stack_Node_Ptr new_node_ptr; // Declare a pointer to a node.

  // Use malloc to allocate memory to the new node pointer.
  new_node_ptr = malloc(sizeof(Stack_Node));

  // If memory is assigned successfully, assign 'value' to member 'data' and
  // assign the pointer to the previous top node to it's pointer member.
  if (new_node_ptr != NULL){
    new_node_ptr->data = value;
    new_node_ptr->next_node_ptr = *top_ptr;

    // Assign 'top_ptr' to the new node, which is at the top of the stack.
    *top_ptr = new_node_ptr;
  }
  // If memory was not allocated successfully, print error message.
  else {
    printf("%s/n", "Memory was not allocated.");
  }
}

// ******************************************************************************
// *                             Function 'pop'                                 *
// ******************************************************************************
char pop(Stack_Node_Ptr *top_ptr)
{
  char value; // Value to be returned.
  // Assign the pointer to the top node to 'temp_ptr'.
  Stack_Node_Ptr temp_ptr = *top_ptr; 

  // Assign the value in the top node to 'value'.
  value = (*top_ptr)->data;
  // Point 'top_ptr' at the next node in the stack.
  *top_ptr = (*top_ptr)->next_node_ptr;

  // Free the memory associated with the top node.
  free(temp_ptr);

  // Return the value in the popped node.
  return value;
}

// ******************************************************************************
// *                             Function 'stack_top'                           *
// ******************************************************************************
char stack_top(Stack_Node_Ptr top_ptr)
{
  char value; // Value in the top node.

  value = top_ptr->data;

  return value; // Return the value in the top node.
}

// ******************************************************************************
// *                             Function 'is_empty'                            *
// ******************************************************************************
int is_empty(Stack_Node_Ptr top_ptr)
{
  // If the top node is NULL, the stack is empty, so return 1.
  // Otherwise, return 0.
  if (top_ptr == NULL) {
    return 1;
  }
  else {
    return 0;
  }
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
