//  *****************************************************************************
//  *
//  *    sandbox.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Fri Aug 21 08:10:01 2020
//  *
//  *****************************************************************************
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int *cal;
  int *mal;
  int size = 10;
  {
    cal = calloc(size, sizeof(int));
    mal = malloc(size * sizeof(int));

    for (size_t i = 0; i < size; i++) {
      cal[i] = i;
      mal[i] = 10 + i;
    }

    for (size_t i = 0; i < size; i++) {
      printf("%s%ld%s%d\t", "cal[", i, "]:", cal[i]);
      printf("%s%ld%s%d\n", "mal[", i, "]:", mal[i]);
    }

    size++;
  
    cal = realloc(cal, size * sizeof(int));
    mal = realloc(mal, size * sizeof(int));
    puts("");
  }
  for (size_t i = 0; i < size; i++) {
    printf("%s%ld%s%d\t", "cal[", i, "]:", cal[i]);
    printf("%s%ld%s%d\n", "mal[", i, "]:", mal[i]);
  }  
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
