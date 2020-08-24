//  *****************************************************************************
//  *
//  *    sandbox.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Fri Aug 21 08:10:01 2020
//  *
//  *****************************************************************************
#include <stdio.h>
#define PRINTED 90

int main(void)
{
  char character = '+';
  unsigned int non_data;

  puts("Enter choice: ");
  scanf("%u", &non_data);
  
  // Print line of non-data characters (ie a line of asterisks, etc.).
  for (unsigned int i = 0; i < 2 * PRINTED; i++) {

    if (((i + 1) % PRINTED == 0) &&
        (((non_data == 0 || non_data == 1) && (i < PRINTED)) ||
	 ((non_data == 2 || non_data == 3) && (i >= PRINTED)))) {
      character = '\n';      
    }
    
    /* if ((((i + 1) % PRINTED == 0) && (i < PRINTED)) || */
    /*     ((i + 1) % PRINTED == 0) && (i >= PRINTED) && */
    /* 	((non_data == 2) || (non_data == 3))) { */
    /*   character = '\n'; */
    /* } */

    /*   character = ' '; */
    /* } */
    else {
      printf("%c", character);
    }
  }
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
