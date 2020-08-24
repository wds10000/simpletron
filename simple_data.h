//  *****************************************************************************
//  *
//  *    simple_data.h -- 
//  *    Author: Wade Shiell
//  *    Date Created: Sun Aug 23 09:37:35 2020
//  *
//  *****************************************************************************

#ifndef SIMPLE_DATA_H
#define SIMPLE_DATA_H

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

enum mode_list {NOTHING, VIEW, DELETE, LOAD_MAN, LOAD_FILE, SAVE_FILE,
		VIEW_INSTR, EDIT, RUN, PR_REG, PR_MEM, PR_DUMP, SAVE_DUMP, QUIT};

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************