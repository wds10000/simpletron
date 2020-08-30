//  *****************************************************************************
//  *
//  *    simpletron.c -- Simulates execution of a 'SML' (Simpletron Machine
//  *                    Language) program on the Simpletron computer
//  *                    simulator.
//  *    Date Created: Sun Aug 16 16:46:21 2020
//  *
//  *****************************************************************************

#include "SML_simulator.h"

//  *****************************************************************************
//  ***                     Function 'main'                                   ***
//  *****************************************************************************
int main(int argc, char **argv)
{
  // Pass command line argument indicating the file to be executed
  // to 'run_simpletron'.
  run_simpletron(argv[1]);
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
