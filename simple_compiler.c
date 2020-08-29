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
  bool go_to; // Indicates whether the current 'simple' statement is a
              // conditional statement or a 'GOTO' statement.
  bool less_equal; // Indicates whether 'token_value' == 'LESS_EQUAL'.
  bool greater_equal; // Indicates whether 'token_value' == 'GREATER_EQUAL'.
  unsigned int op1; // First operator of equality operator.
  unsigned int op2; // Second operator of equality operator.
  unsigned int temp; // Holds equality operator operand while swapping.

  unsigned int *table_index_ptr; // Pointer to 'table_index'.
  unsigned int *repeat_value_ptr; // Pointer to 'repeat_value'.
  unsigned int *symbol_counter_ptr; // Pointer to 'symbol_counter'.
  table_index_ptr = &table_index;
  repeat_value_ptr = &repeat_value;
  symbol_counter_ptr = &symbol_counter;


  // ***************** vv COMMENT OUT TO SUPRESS PRINTING vv *******************

  printf("\n\t\t\t\t%s\n\n\t\t\t\t%s\n\t\t\t\t%2c\t%2c\t%2c\n\t\t\t\t%s\n\n",
	 "    Symbol Table",
	 "*******************",
	 'S', 'T', 'L',
	 "*******************");

  // ***************** ^^ COMMENT OUT TO SUPRESS PRINTING ^^ *******************
  
  // Iterate through the 'simple' statements in compiler memory & tokenise each.
  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
    statement_position = 0; // Reset statement_position position.
    token_position = 0; // Reset token position.
    // Set initial values for conditional/GOTO variables.
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

    // Compare each token in the passed string to the various 'simple' keywords,
    // variables and constant in order to decide how to process the token.
    while (*tokenised_string != NULL) {
      // Assign 'token_value' according to the nature of the current token.
      token_value = compare_token(*tokenised_string, token_position, SML_memory,
				  compiler_memory, symbol_table, flag,
				  repeat_value_ptr, table_index_ptr, go_to);

      // ****************************** REMARK *********************************
      
      // If line is a remark, exit the loop.
      if (token_value == REMARK) {
      	break;
      }

      // ********************* REPEATED CONSTANT/VARIABLE **********************
      
      // If the token is a repeated constant or variable, assign
      // 'repeat_value_ptr' to 'table_index_ptr'.
      if (token_value == -1 || token_value == -2 || token_value == -3) {
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

      // *********************** LOAD 'PRINT' INSTRUCTION **********************
      
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

      // *********************** LOAD 'PRINT' INSTRUCTION **********************

      

      // ************************ CONDITIONAL STATEMENTS ***********************

      // If the current statement is a conditonal-goto instruction,
      // set 'branch' = true.
      if (token_value == IF) {
    	branch = true;
      }

      // Assign the memory addresses of the operands to 'op1' & 'op2'.
      if (branch == true) {

	if (token_value == VARIABLE || token_value == -3) {

	  // Initialise 'LOAD' instruction & assign variable to 'op1'.
	  if (op1 == -1) {
	    // Initialise 'LOAD' instruction & increment 'instruction_counter'.
	    SML_memory[instruction_counter] = 100 * LOAD;
	    instruction_counter++;

	    // Initialise 'SUBTRACT' instruction & increment 'instruction_counter'.
	    SML_memory[instruction_counter] = 100 * SUBTRACT;
	    instruction_counter++;

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

	case LESS_THAN:
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  break;
	case LESS_EQUAL:
	  less_equal = true; // Set true if 'token' == 'LESS_EQUAL'.

	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  instruction_counter++;
	  
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  break;
	case EQUAL:
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  break;
	case GREATER_EQUAL:
	  greater_equal = true; // Set true if 'token' == 'GREATER_EQUAL'.

	  // Swap operands, which turns 'GREATER_EQUAL' into 'LESS_EQUAL'.
	  temp = op1;
	  op1 = op2;
	  op2 = temp;
	    
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
	  instruction_counter++;
	  
	  // If value in accumulator is equal to zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHZERO;
	  break;
	case GREATER_THAN:
	  // Swap operands, which turns 'GREATER_THAN' into 'LESS_THAN'.
	  temp = op1;
	  op1 = op2;
	  op2 = temp;
	    
	  // If value in accumulator is less than zero, branch.
	  SML_memory[instruction_counter] = 100 * BRANCHNEG;
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
	  // location of current line number to instructions at 'instruction_counter - 3',
	  // 'instruction_counter -2', 'instruction_counter - 1' and 'instruction_counter'.
	  if(less_equal == true || greater_equal == true) {
	    // Add 'op1' to the 'LOAD' instruction.
	    SML_memory[instruction_counter - 3] += op1;

	    // Add 'op2' to the 'SUBTRACT' instruction.
	    SML_memory[instruction_counter - 2] += op2;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string)) {
	      // Add memory location of current line number to first branch instruction.
	      SML_memory[instruction_counter] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[instruction_counter] = atoi(*tokenised_string);	     
	    }
	    instruction_counter++;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string)) {	    
	      // Add memory location of current line number to second branch instruction.
	      SML_memory[instruction_counter] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[instruction_counter] = atoi(*tokenised_string);	     
	    }
	    instruction_counter++;
	  }
	  else {
	    // Add 'op1' to the 'LOAD' instruction.
	    SML_memory[instruction_counter - 2] += op1;

	    // Add 'op2' to the 'SUBTRACT' instruction.
	    SML_memory[instruction_counter - 1] += op2;

	    // If the line number after 'goto' is present in the symbol table, add
	    // the SML memory location referred to by the line 'location' to the branch
	    // instruction. Else, add the line number to the element of 'flag' with the
	    // same index as 'symbol_table'.
	    if (search_table(symbol_table, LINE, *tokenised_string)) {	    
	      SML_memory[instruction_counter] += symbol_table[*repeat_value_ptr].location;
	    }
	    // If the line number after 'goto' references a line ahead of the
	    // current position, add the line number to the element 'flag'
	    // corresponding to the same element in 'SML_memory'.
	    else {
	      flag[instruction_counter] = atoi(*tokenised_string);	     
	    }
	    instruction_counter++;	    
	  }
	}
	else {
	  // Initialise 'BRANCH' instruction, then add operand in SML memory.
	  SML_memory[instruction_counter] = 100 * BRANCH;

	  // If the line number after 'goto' is present in the symbol table, add
	  // the SML memory location referred to by the line 'location' to 'BRANCH'
	  // instruction. Else, add the line number to the element of 'flag' with the
	  // same index as 'symbol_table'.
	  if (search_table(symbol_table, LINE, *tokenised_string)) {	  
	    SML_memory[instruction_counter] += symbol_table[*repeat_value_ptr].location;
	  }
	  // If the line number after 'goto' references a line ahead of the
	  // current position, add the line number to the element 'flag'
	  // corresponding to the same element in 'SML_memory'.
	  else {
	    flag[instruction_counter] = atoi(*tokenised_string);	     
	  }
	  instruction_counter++;
	}
      }

      // *************************** END STATEMENT *****************************

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

  printf("\n\t\t\t\t%s\n\n", "    Memory Dump");

  for (unsigned int i = 0; i < 86; i++) {
    printf("%s", "*");
  }
  printf("\n%c", '\t');

  for (unsigned int i = 0; i < 10; i++) {
    printf("%2d\t", i);
  }
  puts("");

  for (unsigned int i = 0; i < 86; i++) {
    printf("%s", "*");
  }
  puts("");

  /* // SML memory. */
  for (size_t i = 0; i < SML_MEM_SIZE; i++) {

    if (i % 10 == 0) {
      printf("  %ld%s", i / 10, " | ");
    }

    if (SML_memory[i] >= 97 && SML_memory[i] <= 122) {
      printf("\t %c", SML_memory[i]);
    }
    else {
      printf("\t %d", SML_memory[i]);
    }
    if ((i + 1) % 10 == 0) {
      puts("");
    }
  }

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
			   unsigned int *repeat_value_ptr,
			   unsigned int *table_index_ptr, bool go_to)
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
    if (go_to == false) { // If token is not a line number following 'goto'.
      if (atoi(token)) { // If the token is a number other than 0.
	// Search table for 'token'.
	if (!(*repeat_value_ptr = search_table(symbol_table, CONST, token))) {
	  // If present, return CONST.
	  return CONST;
	}
	else {
	  return -1 * CONST; // If symbol present in table, return -1.
	}
      }
      if (!strcmp(token, "0")) { // If the token is 0.
	// Search table for 'token'.
	if (!(*repeat_value_ptr = search_table(symbol_table, ZERO, token))) {
	  // If present, return ZERO.
	  return ZERO;
	}
	else {
	  return -1 * ZERO; // If symbol present in table, return -1.
	}
      }
    }
    else {
      *repeat_value_ptr = search_table(symbol_table, LINE, token);
      return LINE;
    }
  }
  if (strlen(token) == 1) { // If token is a variable (ie. alphabetic character).
    if (*token >= 97 && *token <= 122) {
      // Search table for 'token'.
      if (!(*repeat_value_ptr = search_table(symbol_table, VARIABLE, token))) {
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
    if (!(*repeat_value_ptr = search_table(symbol_table, LINE, token))) {
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
  // it is not present, retun count.
  while (count < SYMBOL_SIZE) {

    switch (symbol_code) {
      
    case 1: // If the symbol is a constant.
      if (symbol_table[count].type == 'C') {
	if (atoi(token) == symbol_table[count].symbol) {
	  return count;
	}
      }
      break;
    case 2: // If the symbol is a constant (0).
      if (0 == symbol_table[count].symbol && 'C' == symbol_table[count].type) {
	/* return symbol_table[count].location; */
	return count;
      }
      break;
    case 3: // If the symbol is a variable (alphabetic character constant).
      if (token[0] == symbol_table[count].symbol) {
	return count;
      }
      break;
    case 4: // If the symbol is a line number.
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

  if (token_value == VARIABLE) {
    printf("\t\t\t\t%2c\t%2c\t%02d\n", (char) symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
    
  }
  else if (symbol_table[count].symbol != '0') {
    printf("\t\t\t\t%2d\t%2c\t%02d\n", symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
  }
  else {
    printf("\t\t\t\t%2c\t%2c\t%02d\n", symbol_table[count].symbol, symbol_table[count].type,
  	   symbol_table[count].location);
  }

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
