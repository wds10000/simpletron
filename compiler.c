//  *****************************************************************************
//  *
//  *    compiler.c -- Compiles programs written in 'simple'.
//  *    Author: Wade Shiell
//  *    Date Created: Wed Aug 26 10:21:03 2020
//  *
//  *****************************************************************************

#include "simple_compiler.h"

int main(int argc, char **argv)
{
  // Loads a 'simple' program file and compiles it to SML, then saves the
  // resulting 'machine-ready' code to disk.
  compile(argv[1]);
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
