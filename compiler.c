//  *****************************************************************************
//  *
//  *    compiler.c -- Compiles programs written in 'simple'.
//  *    Date Created: Wed Aug 26 10:21:03 2020
//  *
//  *****************************************************************************

#include "simple_compiler.h"

int main(int argc, char **argv)
{
  // Need to ignore line numbers unless they are the first token in a statement.
  // Make sure that line numbers are saved in symbol table properly (ie. symbol).
  // Don't increment instruction counter if the line is a remark.
  // Clean out redundant parameters from function headers.
  // Rename parameters and arguments as appropriate (currently they're the same).

  // Loads a 'simple' program file and compiles it to SML, then saves the
  // resulting 'machine-ready' SML code to disk.

  if (argv[1] != NULL) {
    compile(argv[1]);
  }
  else {
    printf("%s\n", "No argument provided. Enter file name after './compiler'.");
  }
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
