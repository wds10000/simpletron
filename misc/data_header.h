//  *****************************************************************************
//  *
//  *    data_header.h -- 
//  *    Author: Wade Shiell
//  *    Date Created: Sun Aug 23 09:37:35 2020
//  *
//  *****************************************************************************

#ifndef DATA_HEADER_H
#define DATA_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#define REGISTER_LENGTH 1000 // Define maximum size of string 'register_string'.
#define FILE_LENGTH 20 // Define maximum length of program file name.
#define MEMORY_SIZE 1000 // Define size of Simpletron 'memory' 
#define SENTINEL -0XFFFFF // Define sentinel value.
#define PRINTED 90 // Length of inside strings in prompts and information.
#define RED "\033[0;31m" // Define string to change colour to red.
#define GREEN "\033[0;32m" // Define string to change colour to green.
#define YELLOW "\033[0;33m" // Define string to change colour to yellow.
#define BLUE "\033[0;34m" // Define string to change colour to blue.
#define RESET "\033[0m" // Define string to reset colour to normal.

// Enumeration list for initial program mode selection.
// NOTHING - Empty element at list start (disregard).
// VIEW_PRG - View saved programs.
// DEL_PRG - Delete a saved program.
// LOAD_MAN - Enter instructions into 'memory' manually.
// LOAD_FILE - Load instructions from a saved file.
// SAVE_FILE - Save instructions loaded into 'memory' to a file.
// VIEW_INSTR - View the list of instructions currently loaded into 'memory'.
// EDIT_INSTR - Edit the list of instructions currently loaded into 'memory'.
// RUN_PRG - Execute the list of instructions currently loaded into 'memory'.
// PRT_REG - Print registers for instructions currently loaded into 'memory'.
// PRT_MEM - Print 'memory' dump.
// PRT_DUMP - Print 'memory' dump as well as registers.
// SAVE_MEM - Save 'memory' dump to a dump file.
// QUIT - Quits the Simpletron program.

enum mode_list {NADA, VIEW_PRG, DEL_PRG, LOAD_MAN, LOAD_FILE, SAVE_FILE,
		VIEW_INSTR, EDIT_INSTR, RUN_PRG, PRT_REG, PRT_MEM, PRT_DUMP,
		SAVE_MEM, QUIT};

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
