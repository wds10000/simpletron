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
  char *compiler_memory[COMPILER_MEM_SIZE]; // Compiler memory.
  Symbols symbol_table[SYMBOL_SIZE]; // Holds symbol data during compilation
  int flag[COMPILER_MEM_SIZE]; // Holds flags to unresolved references.
  unsigned int SML_memory[COMPILER_MEM_SIZE]; // Holds compiled SML instructions.

  unsigned int data_counter = 0;
  unsigned int instruction_counter = 0;
  
  // Initialise the symbol table and flag array.
  initialise(compiler_memory, symbol_table, flag);

  // Load a 'simple' program into compiler memory.
  load_program(compiler_memory, simple_file);

  // Perform first pass on 'simple' statements saved in 'compiler_memory'.
  first_pass(compiler_memory, symbol_table, flag);
  /* // Perform second pass on 'simple' statements saved in 'compiler_memory'. */
  /* second_pass(compiler_memory, symbol_table, flag); */
  /* // Save compiled 'SML' code to disk. */
  /* save_file(SML_memory, simple_file); */
}

// ******************************************************************************
// *                             Function 'initialise'                          *
// ******************************************************************************
void initialise(char *compiler_memory[], Symbols symbols_table[], int flag[])
{
  // Iterate through 'symbols_table' and 'flag', initialising all elements.
  for (size_t i = 0; i < SYMBOL_SIZE; i++){
    symbols_table[i].symbol = 00;
    symbols_table[i].type = '\0';
    symbols_table[i].location = 00;

    flag[i] = -1;
  }

  // Iterate through 'compiler_memory', initialising all elements.
  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++){
    compiler_memory[i] = "";
  }
}

// ******************************************************************************
// *                             Function 'load_program'                        *
// ******************************************************************************
void load_program(char *compiler_memory[], char simple_file[])
{
  FILE *file_ptr; // Declare new file pointer

  // If file was successfully loaded, assign each 'simple' statement contained
  // therein to 'compiler_memory'.
  if ((file_ptr, fopen(simple_file, "r+")) != NULL) {

    for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
      fscanf(file_ptr, "%s", compiler_memory[i]);
    }
  }
  // If file was not successfully loaded, print an error message.
  else {
    printf("%s\n", "File was not loaded successfully.");
  }
}

// ******************************************************************************
// *                             Function 'first_pass'                          *
// ******************************************************************************
void first_pass(char *compiler_memory[], Symbols symbols_table[], int flag[])
{
  // Iterate through the 'simple' statements in compiler memory & tokenise each.

  for (size_t i = 0; i < COMPILER_MEM_SIZE; i++) {
    tokenise(compiler_memory);

    // Use v-l array to return an array of tokens, then process the array in
    // this function
    
    /* convert_infix(); */
    /* evaluate_postfix(); */
  }
}

void tokenise(char *simple_statement)
{
  char *token_ptr = strtok(compiler_memory, " ");
  
  while (token_ptr != NULL){
    compare_token(token_ptr);
    token_ptr = strtok(NULL, " ");
  }
}

void compare_token(char *token)
{
  if (token == "rem") {
    
  }

  // Same for other keywords.
  
  /* int a; */
  /* switch (a) { */
  /* case REM: */
  /*   break; */
  /* case INPUT: */
  /*   break; */
  /* case LET: */
  /*   break; */
  /* case IF: */
  /*   break; */
  /* case LESS_THAN: */
  /*   break; */
  /* case LESS_EQUAL: */
  /*   break; */
  /* case EQUAL: */
  /*   break; */
  /* case GREAT_EQUAL: */
  /*   break; */
  /* case GREAT_THAN: */
  /*   break; */
  /* case GOTO: */
  /*   break; */
  /* case END: */
  /*   break; */
  /* default: */
  /*   break; */
  /* } */
}
/* // ****************************************************************************** */
/* // *                             Function 'second_pass'                         * */
/* // ****************************************************************************** */
/* void second_pass(char *compiler_memory[], Symbols symbols_table[], int flag[]) */
/* { */
  
/* } */

/* // ****************************************************************************** */
/* // *                             Function 'save_file'                           * */
/* // ****************************************************************************** */
/* void save_file(unsigned int SML_memory[], char simple_file[]) */
/* { */
  
/* } */

/* // ****************************************************************************** */
/* // *                             Function 'convert_infix'                       * */
/* // ****************************************************************************** */
/* void convert_infix() */
/* { */
  
/* } */

/* // ****************************************************************************** */
/* // *                             Function 'evaluate_postfix'                    * */
/* // ****************************************************************************** */
/* void evaluate_postfix() */
/* { */
  
/* } */
// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
