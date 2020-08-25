//  *****************************************************************************
//  *
//  *    main.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Sun Aug 16 16:46:21 2020
//  *
//  *****************************************************************************

#include <stdio.h>
#include "simple_header.h"
#include "execute_header.h"

int main(void)
{

  // - If input is negative, convert to hex representation of negative number
  // - Check functions & variables for 'const' and correct type (ie. unsigned)
  // - Change register initialisations to hex if appropriate.
  // - Need to change function parameter names so they're not the same as the
  //   corresponding variable names.
  // - Add 'sub-quit' option whenever user is prompted for input (ie. if user
  //   wants to enter the name of a file to open, there should be the ability
  //   to quit without entering a file name even if 'open a file' has been
  //   chosen).
  // - Print slimmed down version of 'mode options' when every 'select mode'
  //   prompt is displayed?
  // - Add check to prevent seg faults and invalid entry for all prompts
  // - Remove 'hard coded' newlines from all instructions and replace with
  // - 'newline' instruction?
  // - Add information for currently loaded program (or 'null') to prompt if
  //   program loaded into memory.
  // - Add error message if instruction entered is invalid.
  // - Need to change instruction size from four digits to six digits because
  //   memory size is now 1000. Or is this taken care of using hex integers?
  // - Will the memory numbering change from 000 - 999 given hex integers
  //   are being used?
  // - Need to add warning when memory is near to being exceeded/has been
  //   exceeded.
  // - Keep a tally on how many memory addresses have been used; keep a register
  //   of used/unused memory locations.
  // - If user enters data which is too big to be stored (contiguously?) in
  //   memory, print error (ie. string too long).
  // - Need to ensure that if string is written, it is the same string that
  //   was just read (otherwise string length will be different).
  // - Need to find a way not to define MEMORY_SIZE in both header files.
  // - Use 'load_register' in 'load_manual' to determine whether either the
  //   instruction type or the operand is invalid.
  // - Add mode which clears loaded memory (zeroes it). Prompts (are you sure?).
  // - After halt instruction is entered, zero all remaining 'memory' locations?
  // - Get segfault if try exectuing stored program twice in a row. Why?
  // - Prompt 'Do you want to save the loaded program to file?' before quitting.
  // - Prompt 'Are you sure' if trying to save over a pre-existing file.
  // - If entering more than one string, need to ensure that the strings
  //   aren't writing over pre-existing memory.
  // - If memory dump is printed before being saved to file, first save it to file
  //   then call the print function from 'save_dump' (add conditional at end of
  //   function which only allows this to print if 'print dump' has first been
  //   requested.
  // - View program: need to control for loop to iterate through instructions
  //   with 'instruction counter' variable. This will be included in the list of
  //   registers for a profram loaded into memory, but it should also be added to
  //   the file to which programs are saved. It can then be used when printing
  //   instructions on screen.
  // - Allow user to bring up list of instructions whenever (mode choice?).
  // - Add description of 'instruction types' when prompting user for
  // - instruction input.
  // - User variable length array?
  // - Add colour.
  // - Remove memory location as well as add.
  // - Need to add warnings for calloc and realloc if memory cannot be assigned.
  // - Add 'refresh screen' after commands so each command is at the top of the
  //   terminal.
  // - Use sequential file storage to save a readable copy of a program, but usse
  //   random access storage to save a copy of a program for processing.
  // - Break 'simple_source.c' up into further source files?
  // - If using variable length arrays, will need to make sure that an array of
  //   appropriate length exists when returning from functions like
  //   'edit_instructions' or 'load_file'.
  // - Replace 'colour' functions with '#define' directives such as
  //   #define NORMAL_COLOR  "\x1B[0m"
  //   #define GREEN  "\x1B[32m"
  //   #define BLUE  "\x1B[34m"
  //   etc.
  
  // Run 'Simpletron Machine Language' program.
  run_simpletron();
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
