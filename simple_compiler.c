//  *****************************************************************************
//  *
//  *    simple_compiler.c -- Source file for the functions called by
//  *                         'compiler.c'.
//  *    Author: Wade Shiell
//  *    Date Created: Wed Aug 26 10:02:08 2020
//  *
//  *****************************************************************************

#include "simple_compiler.h"

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

  // ****** Sort these out *****
  unsigned int data_counter = 0;
  // ******                *****
  
  // Initialise the symbol table and flag array.
  initialise(compiler_memory, SML_memory, symbol_table, flag);

  // Load a 'simple' program into compiler memory.
  load_program(compiler_memory, simple_file);
  // Perform first pass on 'simple' statements saved in 'compiler_memory'.
  first_pass(SML_memory, compiler_memory, symbol_table, flag,
	     instruction_counter);
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
		unsigned int instruction_counter)
{
  char **tokenised_string; // 'simple' statement to be tokenised.
  unsigned int token_value; // Indicates return value for 'compare_token'.
  unsigned int table_index = 0; // Position of element in symbol table.
  unsigned int token_position; // Position of token in string.
  unsigned int repeat_value; // Indicates a token has been repeated.
  unsigned int statement_position; // Indicates whether token follows a keyword.
  unsigned int symbol_counter = 0; // Tracks number of constants and variables.
  unsigned int previous_counter; // Holds the last value of 'instruction_counter'.
  bool branch; // Indicates whether the current 'simple' statement is
               // a conditional statement.

  unsigned int *table_index_ptr; // Pointer to 'table_index'.
  unsigned int *repeat_value_ptr; // Pointer to 'repeat_value'.
  unsigned int *symbol_counter_ptr; // Pointer to 'symbol_counter'.
  table_index_ptr = &table_index;
  repeat_value_ptr = &repeat_value;
  symbol_counter_ptr = &symbol_counter;
  
  // Iterate through the 'simple' statements in compiler memory & tokenise each.
  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
    statement_position = 0; // Reset statement_position position.
    token_position = 0; // Reset token position.
    branch = false;
    
    // If 'compiler_memory' is empty, exit the for loop.
    if (!strcmp(compiler_memory[i], "(empty)")) {
      break;
    }
    
    // Tokenise 'compiler_memory[i]'
    tokenised_string = tokenise(tokenised_string, compiler_memory[i]);

    // Compare each token in the passed string to the various 'simple' keywords,
    // variables and constant in order to decide how to process the token.
    while (*tokenised_string != NULL) {
      // Assign 'token_value' according to the nature of the current token.
      token_value = compare_token(*tokenised_string, token_position, SML_memory,
				  compiler_memory, symbol_table, flag,
				  repeat_value_ptr, table_index_ptr);

      // ******************** REMARK *******************************************
      
      // If line is a remark, exit the loop.
      if (token_value == REMARK) {
      	break;
      }

      // ******************** REPEATED CONSTANT/VARIABLE ***********************
      
      // If the token is a repeated constant or variable, assign
      // 'repeat_value_ptr' to 'table_index_ptr'.
      if (token_value == -1 || token_value == -2 || token_value == -3) {
	*table_index_ptr = *repeat_value_ptr;
      }

      // ******************** POPULATE SYMBOL TABLE ****************************
      
      // Assign line numbers, constant and variables to 'symbol_table'.
      if (token_value == CONST) { // Assign constant to 'symbol_table'.
      	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr);
      }
      if (token_value == ZERO) { // Assign constant (zero) to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr);
      }
      if (token_value == VARIABLE) { // Assign variable to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr);
      }
      if (token_value == LINE) { // Assign line number to 'symbol_table'.
	*table_index_ptr
	  = add_symbol(token_value, *tokenised_string, symbol_table,
		       symbol_counter_ptr);
      }

      // ******************** LOAD CONSTANT ************************************
      
      // If token is a constant, assign to SML memory.
      if (token_value == CONST || token_value == ZERO) {
	SML_memory[SML_MEM_SIZE - *symbol_counter_ptr - 1]
	  = symbol_table[*table_index_ptr].symbol;
	(*symbol_counter_ptr)++;
      }

      // ******************** LOAD VARIABLE ************************************
      
      // If the token is a variable, assign to SML memory.
      if (token_value == VARIABLE) {
	SML_memory[SML_MEM_SIZE - *symbol_counter_ptr - 1]
	  = symbol_table[*table_index_ptr].symbol;
	(*symbol_counter_ptr)++;	
      }
 
      // ******************** LOAD 'INPUT' INSTRUCTION *************************
      
      // If program asks for input, initialise 'READ' instruction.
      if (token_value == INPUT) {
	SML_memory[instruction_counter] = 100 * READ;
      }

      // If token is a variable and succeeds 'input', add location of operand in
      // SML memory to previous 'READ' instruction.
      if ((token_value == -3 || token_value == VARIABLE) && statement_position == 2 &&
	  SML_memory[instruction_counter] == 100 * READ) {
	SML_memory[instruction_counter] +=
	  symbol_table[*table_index_ptr].location;
	instruction_counter++;
      }    

      // ******************** LOAD 'PRINT' INSTRUCTION *************************
      
      // If program outputs data, initialise 'WRITE' instruction.
      if (token_value == PRINT) {
	SML_memory[instruction_counter] = 100 * WRITE;
      }

      // If token is a variable and succeeds 'output', add location of operand in
      // SML memory to previous 'WRITE' instruction.
      if ((token_value == -3 || token_value == VARIABLE) && statement_position == 2 &&
	  SML_memory[instruction_counter] == 100 * WRITE) {
	SML_memory[instruction_counter] +=
	  symbol_table[*table_index_ptr].location;
	instruction_counter++;
      }

      // ******************** CONDITIONAL STATEMENTS ***************************

      unsigned int temp1 = -1;
      unsigned int temp2 = -1;      
      unsigned int op1;
      unsigned int op2;      
      
      // If the current statement is a goto instruction, set 'branch' = true.
      if (token_value == IF) {
	branch = true;

	// Load operator code into 'LOAD' instruction.
	SML_memory[instruction_counter] = 100 * LOAD;
	instruction_counter++;
      }

      // Assign the memory addresses of the operands to 'temp1' & 'temp2'.
      if (branch == true && (token_value == VARIABLE || token_value == -3)) {

	// Assign first variable in 'simple' statement to 'temp1'.
	if (op1 == -1) {
	  temp1 = symbol_table[*table_index_ptr].location;
	}
	// Assign first variable in 'simple' statement to 'temp1'.
	else {
	  temp2 = symbol_table[*table_index_ptr].location;
	}	  
      }

      // If the memory locations of both operands are stored in 'symbol_table',
      // use a switch statement to determine which branch instructions to load.

      // Load appropriate branch operator code into branch instruction.
	switch (token_value) {
	case LESS_THAN:
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  // Load.
	case LESS_EQUAL:
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  // Load.
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  // Load.
	case EQUAL:
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  // Load.
	case GREATER_EQUAL:
	  // Swap operands
	  // If value in accumulator is less than zero, do not branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  // Load.
	case GREATER_THAN:
	  // Swap operands
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  // Load.
	  // Swap operands
	  // If value in accumulator is equal to zero, do not branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  // Load.
	}
      }
      
      // Load memory location of first operand into accumulator;
      SML_memory[instruction_counter] += op1;
      instruction_counter++;

      // Load operator code into 'SUBTRACT' instruction.
      SML_memory[instruction_counter - 1] = 100 * SUBTRACT;
      instruction_counter++;

      // Subtract memory location of second operand from accumulator.
      SML_memory[instruction_counter - 1] += op2;
      instruction_counter++;	
	
      // If 'token_value' ==  'GOTO', load operator code into 'BRANCH'
      // instruction.
      if (token_value == GOTO) {
	SML_memory[instruction_counter] = 100 * BRANCH;
	
	// Check for unresolved location before adding operand.
	SML_memory[instruction_counter] += symbol_table[*table_index_ptr].location;
      }

      // ******************** END STATEMENT ************************************

      // If program terminates, assign 'HALT' instruction to SML memory.
      if (token_value == END) {
	SML_memory[instruction_counter] = 100 * HALT;
	instruction_counter++;
      }

      // ***********************************************************************
      
      // Increment to the next token.
      tokenised_string++;
      statement_position++;
      token_position++;
    } 
  }

  // *************** DELETE ***************************************
  for (size_t i = 0; i < SML_MEM_SIZE; i++) {
    if (SML_memory[i] >= 97 && SML_memory[i] <= 122) {
      printf("%c\t", SML_memory[i]);
    }
    else {
      printf("%d\t", SML_memory[i]);
    }
    if ((i + 1) % 10 == 0) {
      puts("");
    }
  }
  /* for (size_t i = 0; i < SML_MEM_SIZE; i++) { */
  /*   if (symbol_table[i].symbol >= 97 && symbol_table[i].symbol <= 122) { */
  /*     printf("%c\t", symbol_table[i].symbol); */
  /*   } */
  /*   else { */
  /*     printf("%d\t", symbol_table[i].symbol); */
  /*   } */
  /*   if ((i + 1) % 10 == 0) { */
  /*     puts(""); */
  /*   } */
  /* } */
  /* puts(""); */
  /* for (size_t i = 0; i < SML_MEM_SIZE; i++) { */
  /*     printf("%d\t", symbol_table[i].location); */
  /*   if ((i + 1) % 10 == 0) { */
  /*     puts(""); */
  /*   } */
  /* } */
  // *************** DELETE ***************************************  
}

// ******************************************************************************
// *                             Function 'second_pass'                         *
// ******************************************************************************
void second_pass(unsigned int SML_memory[], char *compiler_memory[],
		 Symbols symbols_table[], int flag[], char simple_file[])
{
  // Save compiled 'SML' code to disk.
  save_file(SML_memory, simple_file);
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
			   unsigned int *repeat_value_ptr,
			   unsigned int *table_index_ptr)
{  
  // ****************************************************************************
  // Need to clean out reduntant paramenters from function header
  // ****************************************************************************
  
  // Compare the passed token to the various 'simple' keywords, variables and
  // constants and call the appropriate function.

  if (!strcmp(token, "rem")) { // If the token is part of a remark.
    return REMARK;  
  }
  else if (token_position != 0) { // If token is a constant.
    if (atoi(token)) { // If the token is a number other than 0.
      // Search table for 'token'.
      if (!(*repeat_value_ptr = search_table(symbol_table, 0, token))) {
	// If present, return CONST.
	return CONST;
      }
      else {
	return -1 * CONST; // If symbol present in table, return -1.
      }
    }
    if (!strcmp(token, "0")) { // If the token is 0.
      // Search table for 'token'.
      if (!(*repeat_value_ptr = search_table(symbol_table, 1, token))) {
	// If present, return ZERO.
	return ZERO;
      }
      else {
	return -1 * ZERO; // If symbol present in table, return -1.
      }
    }
  }
  if (strlen(token) == 1) { // If token is a variable (ie. alphabetic character).
    if (*token >= 97 && *token <= 122) {
      // Search table for 'token'.
      if (!(*repeat_value_ptr = search_table(symbol_table, 2, token))) {
	// If present, return VARIABLE.
	return VARIABLE;
      }
      else {
	return -1 * VARIABLE; // If symbol present in table, return -1.
      }
    }
  }
  if (token_position == 0) { // If token is a line number.
    // Search table for 'token'.
    if (!(*repeat_value_ptr = search_table(symbol_table, 3, token))) {
      // If present, return LINE.
      return LINE;
    }
    else {
      return -1 * LINE; // If symbol present in table, return -1.
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
			  char *token)
{
  unsigned int count = 0; // Controls search iteration.

  // Iterate through the symbol table. If the symbol is present, return 0. If
  // it is not present, retun 1.
  while (count < SYMBOL_SIZE) {

    switch (symbol_code) {
      
    case 0: // If the symbol is a constant.
      if (symbol_table[count].type == 'C') {
	if (atoi(token) == symbol_table[count].symbol) {
	  return count;
	}
      }
      break;
    case 1: // If the symbol is a constant (0).
      if (0 == symbol_table[count].symbol && 'C' == symbol_table[count].type) {
	/* return symbol_table[count].location; */
	return count;
      }
      break;
    case 2: // If the symbol is a variable (alphabetic character constant).
      if (token[0] == symbol_table[count].symbol) {
	return count;
      }
      break;
    case 3: // If the symbol is a line number.
      if (symbol_table[count].type == 'L') {
	if (atoi(token) == symbol_table[count].symbol) {
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
			Symbols symbol_table[], unsigned int *symbol_counter_ptr)
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
    }
  }

  // ********************** DELETE *********************************************
  
  if (token_value == VARIABLE) {
    printf("%c\t%c\t%d\n", (char) symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
    
  }
  else if (symbol_table[count].symbol != '0') {
    printf("%d\t%c\t%d\n", symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
  }
  else {
    printf("%c\t%c\t%d\n", symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
  }

  // ********************** DELETE *********************************************
  
  return count;
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
  puts("");
}

// ******************************************************************************
// *                             Function 'convert_infix'                       *
// ******************************************************************************
void convert_infix(void)
{
  ;
}

// ******************************************************************************
// *                             Function 'evaluate_postfix'                    *
// ******************************************************************************
void evaluate_postfix(void)
{
  ;
}
// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************

/* // If the token indicates an assignment, */
/* if (token_value == LET) { */
/* 	; */
/* } */

/* // If the token indicates a branch statement, */
/* if (token_value == IF) { */
/* 	; */
/* } */

/* // Branch to line XX. */
/* if (token_value == GOTO) { */
/* 	; */
/* } */
      
/* // End program execution. */
/* if (token_value == END) { */
/* 	; */
/* } */

