//  *****************************************************************************
//  *
//  *    simple_source.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Mon Aug 17 15:14:05 2020
//  *
//  *****************************************************************************

#include "simple_header.h"
#include "execute_header.h"

//  *****************************************************************************
//  ***                     Function 'run_simpletron'                         ***
//  *****************************************************************************
void run_simpletron(void)
{  
  // Declare program variables.
  unsigned int instruction_register = 0; // Holds the current instruction.
  unsigned int operation_code = 0; // Indicates the current instruction type.
  int instruction_counter = 0; // Tracks memory location of current instruction.
  int accumulator = 0; // Accumulator registry.
  int operand = 0; // Indicates the 'memory' location currently operated on.
  int memory[MEMORY_SIZE] = {0x0}; // Array which simulates Simpletron memory.
  char dump_file[FILE_LENGTH] = {0}; // Name of ile 'memory' dump is saved to.
  char register_string[REGISTER_LENGTH] = {0}; // Stores registers as a string.

  // Declare pointers.
  unsigned int *instruction_register_ptr;
  unsigned int *operation_code_ptr;
  int *instruction_counter_ptr;
  int *accumulator_ptr;
  int *operand_ptr;
  instruction_register_ptr = &instruction_register;
  operation_code_ptr = &operation_code;
  instruction_counter_ptr = &instruction_counter;
  accumulator_ptr = &accumulator;
  operand_ptr = &operand;

  // Print welcome message and 'Simpletron' user instructions.
  print_welcome();

  unsigned int mode = -1; // Simpletron mode chosen by user.

  // Loop until user quits program (mode = 13).
  do {
    // Prompt the user to chose Simpletron mode.
    printf("\n%s%s\n%s", "***                       -Select mode-",
	   "                          ***",
	   "                              >> ");
    scanf("%d", &mode);
     
    // Choose between program execution options (manual, file, quit).
    switch (mode) {

    case VIEW: // View list of saved programs.
      view_programs(memory);
      // What if no programs present?
      break;
    case DELETE: // Delete a program.
      delete_program();
      // What if no programs present? Or invalid name entered?
      break;
    case LOAD_MAN: // Load program (manually).
      load_manual(memory, instruction_counter_ptr);
      break;
    case LOAD_FILE: // Load program (file).
      load_file(memory, instruction_counter_ptr); 
      // What if invalid name entered?
      break;
    case SAVE_FILE: // Save program to file.
      save_file(memory, register_string, instruction_counter_ptr);
      // What if file already exists?
      // What if no program loaded?      
      break;
    case VIEW_INSTR: // View loaded program instructions.
      view_instructions(memory, instruction_counter_ptr);
      // Add statements.
      // What if no program loaded?
      break;
    case EDIT: // Edit loaded program.
      edit_program(memory, instruction_counter_ptr);
      // What if no program loaded?
      break;
    case RUN: // Run loaded program.
      execute(memory, instruction_register_ptr, operation_code_ptr,
	      instruction_counter_ptr, accumulator_ptr, operand_ptr);
      // What if no program loaded?      
      break;
    case PR_REG: // Print register.
      print_registers(1, register_string,
		      accumulator_ptr, instruction_counter_ptr,
		      instruction_register_ptr, 
		      operation_code_ptr, operand_ptr);
      // What if no program loaded? Will print, but need to print error message.      
      break;
    case PR_MEM: // Print Simpletron 'memory'.
      print_memory_dump(memory, dump_file);
      // What if no program loaded? Will print, but need to print error message.      
      break;
    case PR_DUMP: // Print Simpletron dump ('memory' & registers).
      dump(memory, register_string, dump_file, instruction_register_ptr,
	   operation_code_ptr, instruction_counter_ptr, accumulator_ptr,
	   operand_ptr);
      // What if no program loaded? Will print, but need to print error message.      
      break;
    case SAVE_DUMP: // Save Simpletron dump to file.
      save_dump(memory, dump_file, 0);
      // Add program/file name to header in saved file?
      // What if no program loaded? Will print, but need to print error message.      
      break;
    case QUIT: // Quit 'Simpletron: The Computer Simulator'.
      print_goodbye();
      break;
    default: // Prompts user for further input if invalid choice is made.
      printf("\n%s\n\n",
	     "XXX              "
	     "Invalid choice. Please choose again."
	     "            XXX");
      printf("%s%s\n%s", "***                       -Select mode-",
	     "                          ***",
	     "                              >> ");
      scanf("%d", &mode);
      break;
    }
  } while (mode != 13);
}

//  *****************************************************************************
//  ***                     Function 'view_programs'                          ***
//  *****************************************************************************
void view_programs(int memory[MEMORY_SIZE])
{
  ;
}

//  *****************************************************************************
//  ***                     Function 'delete_program'                         ***
//  *****************************************************************************
void delete_program(void)
{
  ;
}

//  *****************************************************************************
//  ***                     Function 'load_manual'                           ***
//  *****************************************************************************
void load_manual(int memory[MEMORY_SIZE], int *instruction_counter_ptr)
{
  // Set number of instructions to zero.
  *instruction_counter_ptr = 0;
  
  int location_number = 0; // Location number in Simpletron 'memory'.
  int instruction = 0X0; // User-entered instruction (may be invalid).

  int *location_number_ptr; // Pointer to 'location_number'.
  int *instruction_ptr; // Pointer to 'instruction'.
  instruction_ptr = &instruction; 
  location_number_ptr = &location_number;

  // Display instructions on how to manually enter a SML program into 'memory'.
  print_entry_prompt();
  
  // Prompt user for program instruction input.
  enter_instruction(instruction_ptr, location_number_ptr,
		    instruction_counter_ptr);
  
  // Print message if sentinel entered (program will terminate).
  if (instruction == SENTINEL) {
    printf("\n%s\n\n",
	   "***               "
	   "No program instructions entered"
	   "                ***");
  }

  // Continue entering instructions until the sentinel value is entered.
  while (instruction != SENTINEL) {
    // Print message if instruction entered is invalid and prompt user
    // to enter instruction again.
    invalid_entry(instruction_ptr, location_number_ptr,
		  instruction_counter_ptr);

    // Store entered instruction in 'memory'.
    memory[location_number] = instruction;

    location_number++; // Move to the next 'memory' location.

    // Prompt user for program instruction input, and store in 'memory' unless
    // sentinel value is entered.
    enter_instruction(instruction_ptr, location_number_ptr,
		      instruction_counter_ptr);

    // Print message if instruction entered is invalid and prompt user
    // to enter instruction again.
    invalid_entry(instruction_ptr, location_number_ptr,
		  instruction_counter_ptr);
  }

  // Print appropriate message if instructions were entered *before* the 
  // sentinel value was input.
  if (location_number != 0) {
    printf("\n%s\n",
	   "***          "
	   "Program instructions entered successfully."
	   "          ***");
  }
}

//  *****************************************************************************
//  ***                     Function 'load_file'                              ***
//  *****************************************************************************
void load_file(int memory[MEMORY_SIZE], int *instruction_counter_ptr)
{
  FILE *load_ptr; // Pointer to file containing program instructions.
  char loaded_file[FILE_LENGTH]; // Name of file program loaded from.

  // Prompt user to enter file name.
  printf("%s%s\n%s", "***                -Enter name of file to load-",
	 "                  ***",
	 "                              >> ");
  scanf("%s", loaded_file);

  // Open file chosen by user (read mode "r+").
  if ((load_ptr = fopen(loaded_file, "r+")) != NULL) {
    // Ignore header characters.
    char ignore_char; // Catches characters to ignore.
      
    for (unsigned int i = 0; i < 251; i++) {
      fscanf(load_ptr, "%c", &ignore_char);	
    }

    // Save only instructions to 'memory' - ignore preceding characters at the
    // beginning of each instruction (ie '0--:   ').
    char ignore_string[8]; // Catches strings to ignore.
      
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
      fscanf(load_ptr, "%8s", ignore_string);
      fscanf(load_ptr, "%6X", &memory[i]); // Load instruction to memory.

      // Ignore newline at the end of each line of instructions.
      if ((i + 1) % 5 == 0) {
	fscanf(load_ptr, "%c", &ignore_char);
      }
    }

    //    Extract and store the number of instructions saved.
    char catch[13];
    char pr_char;

    /* for (unsigned int i = 0; i < MEMORY_SIZE; i++) { */
    /*   fscanf(load_ptr, "%s", catch); */
    /* } */
    rewind(load_ptr);
    
    for (unsigned int i = 0; i < 87 * 200 - 117 - 253 + 17 + 68 * 6; i++) {
      fscanf(load_ptr, "%c", &pr_char);
    }   
    for (unsigned int i = 0; i < 52; i++) {
      printf("%c", pr_char);
      fscanf(load_ptr, "%c", &pr_char);
    }   
    
    puts("");
    fclose(load_ptr); // Close file 'loaded_file'.
  }
  // If file 'loaded_file' cannot be opened, print error message.
  else {
    printf("%s%s%s\n", "XXX Program file \"", loaded_file,
	   "\" could not be opened. XXX");
  }
}

//  *****************************************************************************
//  ***                     Function 'save_file'                              ***
//  *****************************************************************************
void save_file(int memory[MEMORY_SIZE], char* register_string,
	       int *instruction_counter_ptr)
{
  FILE *save_ptr; // Pointer to file program saved to.
  char saved_file[FILE_LENGTH]; // Name of file program saved to.
  
  // Prompt user to enter file name.
  printf("%s%s\n%s", "***                   -Enter name for saved file-",
	 "                ***",
	 "                              >> ");
  scanf("%s", saved_file);

  // Open file 'saved_file' for writing (write mode "w+").
  if ((save_ptr = fopen(saved_file, "w+")) != NULL) {

    // Start print header for file "saved_file".
    for (unsigned int i = 0; i < 82; i++) {
      fputc('*', save_ptr);
    }
    fprintf(save_ptr, "%c", '\n');

    for (size_t j = 1; j < 6; j++) {
      fprintf(save_ptr, "%-8s%-9s", "#", "Instr.");
    }
    fprintf(save_ptr, "%c", '\n');
	  
    for (unsigned int i = 0; i < 82; i++) {
      fputc('*', save_ptr);
    }
    fprintf(save_ptr, "%c%c", '\n', '\n');
    // End print header for file "saved_file".

    // Print program instructions to file "saved_file".
    for (size_t i = 0; i < MEMORY_SIZE; i++) {

      // If value stored in memory is a (non-variable) instruction, save.
      if (memory[i] != 0) {
	fprintf(save_ptr, "%s%03ld%s%-7X", "#", i, ":   0X", memory[i]);
      }
      // If value stored in memory is a variable (or zero), save as '0000'.
      else if ((memory[i] == 0) && (i < *instruction_counter_ptr)) {
	fprintf(save_ptr, "%s%03ld%s%4.4d%3c", "#", i, ":   0X", 0000, ' ');	
      }
      // If no further valid instructions have been entered, store '----'.
      else {
	fprintf(save_ptr, "%s%03ld%s%s%3c", "#", i, ":   0X", "----", ' ');		
      }

      // Print newline character at the end of each line.
      if ((i + 1) % 5 == 0) {
	fprintf(save_ptr, "%c", '\n');
      }
    }
    puts("");

    // Add instruction count to the bottom of the saved file.
    fprintf(save_ptr, "\n%-31s%-18s%-3d%29s\n", "*", "Instruction Count: ",
	    *instruction_counter_ptr, "*");
    
    fclose(save_ptr); // Close file 'saved_file'.
  }
  // If file 'saved_file' cannot be opened, print error message.
  else {
    printf("%s%s%s\n", "XXX Program file \"", saved_file,
	   "\" could not be saved. XXX");
  }
}

//  *****************************************************************************
//  ***                     Function 'view_instructions'                      ***
//  *****************************************************************************
void view_instructions(int memory[MEMORY_SIZE], int *instruction_counter_ptr)
{
  // If 'memory' is empty, print appropriate message.
  if (*instruction_counter_ptr == 0) {
    printf("\n%-22s%s%21s\n", "*** ","-Simpletron Memory Empty-", " ***");    
  }
  // If 'memory' contains instructions, print in formatted list.
  else {
    printf("\n%-22s%s%21s\n\n", "*** ","-Current Instruction List-", " ***");
  
    // Print until '*instruction_counter_ptr' instructions printed.
    for(size_t i = 0; i < *instruction_counter_ptr; i++) {

      // If value stored in memory is a (non-variable) (or zero), print.
      if (memory[i] != 0) {
	printf("%25s%03ld%s%X\n", "", i, " +", memory[i]);
      }
      // If value stored in memory is a variable (or zero), print '0000'.
      else if ((memory[i] == 0) && (i < *instruction_counter_ptr)) {
	printf("%25s%03ld%s%4.4d\n", "", i, " +", 0000);
      }
      // If no further valid instructions have been entered, print '----'.
      else {
	printf("%25s%03ld%s%s\n", "", i, " +", "----");
      }
    }
  }
}

//  *****************************************************************************
//  ***                     Function 'edit_program'                           ***
//  *****************************************************************************
void edit_program(int memory[MEMORY_SIZE], int *instruction_counter_ptr)
{
  // Declare 'temp_memory1', which acts as a pointer to a variable length array.
  // Use 'calloc', 'realloc' and 'free' to alter the size of the array as
  // necessary to add and subtract memory locations.
  int *temp_memory;
  
  // Declare 'temp_counter' to refer to the number of elements in 'temp_memory'
  // and initially assign to it the value of '*instruction_counter_ptr'.
  int temp_counter = 0;
  temp_counter = *instruction_counter_ptr;

  // Create an array of length 'temp_counter'. 'calloc' initialises the array to
  // zero.
  temp_memory = calloc(temp_counter, sizeof(int));

  // Iterate through 'memory' and copy instructions into 'temp_memory'
  // (locations in 'memory' not containing instructions are ignored,
  // ie. elements beyond 'temp_counter').
  for (size_t i = 0; i < temp_counter; i++) {
    temp_memory[i] = memory[i];
  }

  // Variable 'location' indicates the user's choice of instruction to edit.
  unsigned int location = 0;
  
  // Variable 'add_delete' indicates whether to add or delete a memory location.
  unsigned int add_delete = 0;

  // Variable 'temp_instruction' holds the edited instruction value at a given
  // location in 'temp_memory'.
  unsigned int temp_instruction = 0;
  
  // Variable 'choice' indicates which editing options has been chosen.
  unsigned int choice = 0;
  
  // Prompt user to enter editing choice.
  printf("\n%-8s%s%8s\n%-11s%s%10s\n%-13s%s%13s\n%-8s%s%7s\n%s",
	 "*", "Press 1 to view current list of edited instructions.", "*",
	 "*", "Press 2 to insert/delete a new memory location.", "*",
	 "*", "Press 3 to edit a current memory location.", "*",
	 "*", "Press 0 to quit editing the current instruction list.", "*",
	 "                              >> ");  
  scanf("%d", &choice);

  unsigned int edit = 1; // Variable controls editing loop.
  
  // Keep editing instructions until user quits (0).
  while (edit != 0) {

    switch (choice) {

      // User quits editing.
    case 0: // Assign values in 'temp_memory' to 'memory' once editing complete.
      for (size_t i = 0; i < MEMORY_SIZE; i++) {
	memory[i] = temp_memory[i];
      }

      // Set the number of instructions contained in 'memory' equal to the new
      // number of instructions after ediiting.
      *instruction_counter_ptr = temp_counter;

      // Free memory allocated by 'calloc' and 'realloc' using 'free'.
      free(temp_memory);
      
      edit = 0; // Allows exit from loop.
      break;
    case 1: // View current list of edited instructions.
      view_instructions(temp_memory, &temp_counter);
      break;
    case 2: // Insert new 'memory' location
      //View current list of edited instructions.      
      view_instructions(temp_memory, &temp_counter);

      // Ask whether to add or delete a 'memory' location.
      printf("\n%-21s%s%21s\n%-20s%s%19s\n%s",
	     "*", "1 - Add a memory location.", "*",
	     "*", "2 - Delete a memory location.", "*",
	     "                              >> ");  
      scanf("%d", &add_delete);

      
      // If 'add_delete' = 1, prompt user to enter 'memory' location before
      // one to be added. Otherwise, prompt user to enter 'memory' location
      // to be deleted.
      if (add_delete == 1) {
	printf("\n%-14s%s%15s\n%-22s%s%22s\n%s",
	       "*", "Enter the 'memory' position immediately", "*",
	       "*", "before the new location.", "*",
	       "                              >> ");  
      }
      else {
	printf("\n%-13s%s%13s\n%s",
	       "*", "Enter the 'memory' position to be deleted.", "*",
	       "                              >> ");  	
      }
      scanf("%d", &location);

      // If adding a location to 'memory'.
      if (add_delete == 1) {
	// Use 'realloc' to add a new 'memory' location to 'temp_memory'.
	temp_memory = realloc(temp_memory, (temp_counter + 1) * sizeof(int));

	// Shift 'temp_memory' elements forward by one, working backwards from
	// the last element (temp_memory[temp_counter]) and ending with the new
	// location added.
	for (size_t i = temp_counter; i > location; i--) {
	  temp_memory[i] = temp_memory[i - 1];
	}

	// Initialise the new 'memory' location to zero.
	temp_memory[location + 1] = 0;
      
	// Print confirmation that new location has been added, and print the new
	// instruction list.
	printf("%-14s%s%02X%c%13s\n",
	       "*", "New memory location added at position ",
	       location, '.', "*");
      }

      // If deleteing a location from 'memory'.
      if (add_delete == 2) {
	// Shift 'temp_memory' elements backward by one, starting with the
	// 'memory' location immediately ahead of the location to be deleted.
	for (size_t i = location; i < temp_counter; i++) {
	  temp_memory[i] = temp_memory[i + 1];
	}

	// Initialise the empty 'memory' location to zero.
	temp_memory[temp_counter + 1] = 0;
      
	// Use 'realloc' to remove the empty 'memory' location at the end of
	// 'temp_memory'.
	temp_memory = realloc(temp_memory, (temp_counter - 1) * sizeof(int));
      
	// Print confirmation that new location has been added, and print the new
	// instruction list.
	printf("%-26s%s%02X%s%13s\n",
	       "*", "Memory location ",
	       location, " deleted.", "*");
      }

      // If a new 'memory' location was added before the current end of the
      // instruction list, increment the counter.  If a'memory' location
      // was deleted before the current end of the instruction list, decrement
      // the counter. Otherwise, the new location was added in an empty region
      // of 'memory' so do nothing
      if ((location < temp_counter) && (add_delete == 1)) {
	// Increment 'temp_counter' to reflect the addition of new 'memory'.
	temp_counter++;
      }
      else if ((location < temp_counter) && (add_delete == 2)) {
	temp_counter--;
      }
      
      // Print instruction list.
      view_instructions(temp_memory, &temp_counter);
      
      // Is 'temp_counter' needed in this function anymore?
      
      // Need to print warning if more locations are added than exist as empty
      // locations in 'memory'. Keep counter of empty locations? Remember to change
      // value of counter if, in the editing process, locations in 'memory' are
      // zeroed, thus freeing them.

      // Also need some way of checking whether, after editing, a program still
      // executes properly (assuming it did initially)?
      
      break;
    case 3:// Edit current 'memory' location.
      // Print instruction list.
      view_instructions(temp_memory, &temp_counter);
      
      // Prompt user to enter the location of the instruction to be edited.
      printf("\n%-15s%s%15s\n%-20s%s%19s\n%s",
	     "*", "Enter the 'memory' location containing", "*",
	     "*", "the instruction to be edited.", "*",
	     "                              >> ");  	     
      scanf("%d", &location);

      // Print instruction to be edited.
      printf("\n%25s%03d%s%X\n", "", location, " +", temp_memory[location]);
      
      // Variable 'temp_location' holds a copy of 'temp_memory[location]' while
      // the user edits it.
      int temp_location = temp_memory[location];

      // Prompt user for instruction.
      printf("\n%-23s%s%23s\n", "*", "Enter new instruction:", "*");
      printf("%25s%03d%s", "", location, " +");
      scanf("%X", &temp_instruction);

      unsigned int confirm = 0; // Indicates whether user wants to commit edit.
      printf("\n%-14s%s%15s\n%s", "*", "Press 1 to confirm change, 0 to cancel.", "*",
	     "                              >> ");	     
      scanf("%d", &confirm);

      if (confirm == 1) {
	temp_memory[location] = temp_instruction;
	printf("%-27s%s%27s\n", "*", "Edit confirmed.", "*");	
      }
      else {
	printf("%-27s%s%27s\n", "*", "Edit discared.", "*");
      }

      // Print instruction list.
      view_instructions(temp_memory, &temp_counter);
      break;
    default:
      // Add default statements.
      break;
    }

    // If user has not quit (choice 0), continue editing.
    if (edit != 0) {
      // Prompt user to enter editing choice.
      printf("\n%-8s%s%8s\n%-16s%s%12s\n%-13s%s%13s\n%-8s%s%7s\n%s",
	     "*", "Press 1 to view current list of edited instructions.", "*",
	     "*", "Press 2 to insert a new memory location.", "*",
	     "*", "Press 3 to edit a current memory location.", "*",
	     "*", "Press 0 to quit editing the current instruction list.", "*",
	     "                              >> ");  
      scanf("%d", &choice);
    }
  }
  printf("\n%-20s%s%19s\n",
	 "*", "Instruction editing complete.", "*");
}

//  *****************************************************************************
//  ***                     Function 'execute'                                ***
//  *****************************************************************************
void execute(int memory[MEMORY_SIZE], unsigned int *instruction_register_ptr,
	     unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	     int *accumulator_ptr, int *operand_ptr)
{
  // Reset 'instruction_counter' after a set of program instructions is
  // executed, otherwise the next set of instructions will not start from the
  // beginning of 'memory'.
  *instruction_counter_ptr = 0;

  // Load instruction into 'instruction_register' and seperate it into
  // 'operation_code' and 'operand'.
  load_register(memory, instruction_counter_ptr, instruction_register_ptr,
		operation_code_ptr, operand_ptr);

  int terminate = 0X0; // Variable to control when program execution stops.
  int *terminate_ptr; // Pointer to variable 'terminate'.
  terminate_ptr = &terminate;

  int toggle; // Controls whether instruction counter is incremented (should not
  // be incremented if branch instruction is processed).  
  int *toggle_ptr; // Pointer to variable 'toggle'.
  toggle_ptr = &toggle;

  // Execute instructions stored in Simpletron memory until value of
  // '*terminate_ptr' causes program to stop running.
  while (*terminate_ptr != 0X1) {
    *toggle_ptr = 0X0; // Zero toggle at beginning of each instruction.

    // Switch statement is used to differentiate between instruction types
    // and perform relevant operations.
    switch (*operation_code_ptr) {

    case 0X10: // 'Read' instruction.
      read_instruction(memory, operand_ptr);
      break;
    case 0X11: // 'Write' instruction.
      write_instruction(memory, operand_ptr);
      break;
    case 0X12: // 'Read string' instruction.
      read_string(memory, operand_ptr);
      break;
    case 0X13: // 'Write string' instruction.
      write_string(memory, operand_ptr);
      break;
    case 0X20: // 'Load' instruction.
      load_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X21: // 'Store' instruction.
      store_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X30: // 'Add' instruction.
      add_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X31: // 'Subtract' instruction.
      subtract_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X32: // 'Divide' instruction.
      divide_instruction(memory, operand_ptr, accumulator_ptr, terminate_ptr);
      break;
    case 0X33: // 'Multiply' instruction.
      multiply_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X34: // 'Remainder' instruction.
      remainder_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X35: // 'Exponentiation' instruction.
      exponentiation_instruction(memory, operand_ptr, accumulator_ptr);
      break;
    case 0X39: // 'New line' instruction.
      newline(memory, operand_ptr);
      break;
    case 0X40: // 'Branch' instruction.
      branch_instruction(instruction_counter_ptr, operand_ptr, toggle_ptr);
      break;
    case 0X41: // 'Branch if negative' instruction.
      branchneg_instruction(accumulator_ptr, instruction_counter_ptr,
			    operand_ptr, toggle_ptr);
      break;
    case 0X42: // 'Branch if zero' instruction.
      branchzero_instruction(accumulator_ptr, instruction_counter_ptr,
			     operand_ptr, toggle_ptr);
      break;
    case 0X43: // 'Halt' instruction.
      halt_instruction(terminate_ptr);
      break;
    default:
      // Need to add a default case. Though if there are adequate checks when
      // loading the program from file or manually, there should not be any
      // incorrect instructions being exectued.
      break;
    }

    // If instruction is not a branch, increment the instruction counter.
    increment_counter(toggle_ptr, instruction_counter_ptr);

    // Load instruction into 'instruction_register' and seperate it into
    // 'operation_code' and 'operand'.
    load_register(memory, instruction_counter_ptr, instruction_register_ptr,
		  operation_code_ptr, operand_ptr);
  }
}

//  *****************************************************************************
//  ***                     Function 'print_registers'                        ***
//  *****************************************************************************
void print_registers(bool print_option, char* register_string,
		     int *accumulator_ptr, int *instruction_counter_ptr,
		     unsigned int *instruction_register_ptr,
		     unsigned int *operation_code_ptr, int *operand_ptr)
{
  // Save information for program registers to string 'register_string'.
  sprintf(register_string, "\n%s\n%s\n%s\n%s\n"
	  "%-56s%s%04X%s\n%-56s%5.2X%s\n%-56s%s%4.4X%s\n%-56s%5.2X%s\n%-56s%5.2X"
	  "%s\n%s\n%s\n\n",
	  "********************************************************************",
	  "***                           REGISTERS:                         ***",
	  "********************************************************************",
	  "***                                                              ***",
	  "***    accumulator", "+", *accumulator_ptr, "    ***",
	  "***    instruction_counter", *instruction_counter_ptr, "    ***",
	  "***    instruction_register", "+", *instruction_register_ptr,
	  "    ***",
	  "***    operation_code", *operation_code_ptr, "    ***",
	  "***    operand", *operand_ptr, "    ***",
	  "***                                                              ***",
	  "********************************************************************");
  register_string[REGISTER_LENGTH - 1] = '\0';

  // Don't print registers to the screen if 'print_option' is zero (ie. if
  // registers are being loaded with each execution of an instruction).
  if (print_option == 1) {
    // Print 'register_string'.
    printf("%s", register_string);
  }
}

//  *****************************************************************************
//  ***                     Function 'print_memory_dump'                      ***
//  *****************************************************************************
void print_memory_dump(int memory[MEMORY_SIZE], char* dump_file)
{
  FILE *load_dump_ptr; // Pointer to file 'memory' dump is to be saved to.
  char print_char; // The character in the 'memory' dump currently being printed.
  unsigned int choice; // Choice whether to load from file or 'memory'.

  // Prompt user whether to print 'memory' dump from a file, or from the program
  // currenly loaded into 'memory'.
  printf("%s\n%s\n\n",
	 "1 - Print dump of program currently loaded in Simpletron memory.",
	 "2 - Print dump from saved program.");
  scanf("%d", &choice);

  // If user chooses '1', print from 'memory'. Otherwise print from file.
  switch (choice) {

    // Print from 'memory'.
  case 1:
    // Call 'save_dump' to save the current 'memory' dump to file
    // (fallthrough utilised here).
    save_dump(memory, dump_file, 1);

    // Print from file.
  case 2:
    // If user chooses to print from file, prompt for file name. Otherwise,
    // print from file just saved.
    if (choice == 2) {
      printf("%s%s\n%s", "***                -Enter name for dump file-",
	     "                    ***",
	     "                              >> ");
      scanf("%s", dump_file);
    }
    // Open file 'load_dump_file' for reading (write mode "r+").    
    if ((load_dump_ptr = fopen(dump_file, "r+")) != NULL) {
      // Print the file one character at a time to the screen.
      while (print_char != '@') {
	fscanf(load_dump_ptr, "%c", &print_char);

	// Don't print '@'.
	if (print_char != '@') {
	  printf("%c", print_char);
	}
      } 

      fclose(load_dump_ptr); // Close the file 'load_dump_ptr'.
    }
    // If file 'dump_file' cannot be opened, print error message.      
    else {
      printf("%s%s%s\n", "XXX Dump file \"", dump_file,
	     "\" could not be printed. XXX");
    }
    break;
    // Prompt the user to input again if choice is invalid.
  default:
    printf("\n%s\n\n",
	   "XXX              "
	   "Invalid choice. Please choose again."
	   "            XXX");
    printf("%s\n%s\n\n",
	   "1 - Print dump of program currently loaded in Simpletron memory.",
	   "2 - Print dump from saved program.");
    scanf("%u", &choice);      
    break;
  }
}

//  *****************************************************************************
//  ***                     Function 'dump'                                   ***
//  *****************************************************************************
void dump(int memory[MEMORY_SIZE], char* register_string, char* dump_file,
	  unsigned int *instruction_register_ptr,
	  unsigned int *operation_code_ptr, int *instruction_counter_ptr,
	  int *accumulator_ptr, int *operand_ptr)
{
  print_registers(1, register_string, accumulator_ptr,
		  instruction_counter_ptr, instruction_register_ptr,
		  operation_code_ptr, operand_ptr);
  print_memory_dump(memory, dump_file);
}

//  *****************************************************************************
//  ***                     Function 'save_dump'                              ***
//  *****************************************************************************
void save_dump(int memory[MEMORY_SIZE], char* dump_file, bool print_option)
{
  FILE *dump_ptr; // Pointer to file 'memory' dump is to be saved to.

  // If 'save_dump' is called from 'print_memory_dump' and the user chooses
  // to print from currently loaded 'memory', use a default dump file name
  // so the user does not need to input one (delete the file after?).
  if (print_option == 1) {
    strcpy(dump_file, "temp_dump.txt");
  }
  else {
    // Prompt user to enter file name.
    printf("%s%s\n%s", "***                   -Enter name for dump file-",
	   "                 ***",
	   "                              >> ");
    scanf("%s", dump_file);
  }

  // Open file 'dump_file' for writing (write mode "w+").    
  if ((dump_ptr = fopen(dump_file, "w+")) != NULL) {
    // Don't add the name of the file to the file body if it is only a temporary
    // file (ie. called from 'print_memory_dump' when the user only wanted to
    // print from loaded 'memory'.
    if (print_option != 1) {
      fprintf(dump_ptr, "%23c%s%s%s", ' ', "*** ", dump_file, " ***");
    }

    // Print header to file.
    fprintf(dump_ptr, "\n%s\n%s\n%s\n%s\n",
	    "********************************************************************",
	    "***                            MEMORY:                           ***",
	    "********************************************************************",
	    "*                                                                  *");
    fprintf(dump_ptr, "%c%4s", '*', " ");

    // Print Simpletron 'memory' information as a grid of 'memory' locations,
    // each containing instructions or input.

    // Add 'column' index at top of dump.
    for (unsigned int i = 0; i < 10; ++i) {
      fprintf(dump_ptr, "%6d", i);
    }
    fprintf(dump_ptr, "%s\n", "  *");

    // Iterate through each location in 'memory' and print the stored value
    // in a mxn grid.
    for (size_t index = 0; index < MEMORY_SIZE; ++index) {

      // Add 'row' index at the start of each line.
      if (index % 10 == 0) {
	fprintf(dump_ptr, "%c%4lu", '*', index);
      }

      // Print 'memory' value at location given by 'index'.
      fprintf(dump_ptr, "%s%4.4X", " +", memory[index]);

      // At newline character after every ten instructions.
      if ((index + 1) % 10 == 0) {
	fprintf(dump_ptr, "%s\n", "  *");      
      }
    }

    // Add 'column' index at bottom of dump.
    fprintf(dump_ptr, "%c%4s", '*', " ");

    for (unsigned int i = 0; i < 10; ++i) {
      fprintf(dump_ptr, "%6d", i);
    }
    fprintf(dump_ptr, "%s\n", "  *");      
  
    fprintf(dump_ptr, "%s\n%s\n\n",
	    "*                                                                  *",
	    "********************************************************************");

    // Add 'terminating character' (@) to indicate end of copying.
    fprintf(dump_ptr, "%c", '@');
    
    // Close file 'saved_file'.
    fclose(dump_ptr);
  }
  // If file 'dump_file' cannot be opened, print error message.      
  else {
    printf("%s%s%s\n", "XXX Dump file \"", dump_file,
	   "\" could not be saved. XXX");
  }
}

//  *****************************************************************************
//  ***                   Function 'print_welcome'                            ***
//  *****************************************************************************
void print_welcome(void)
{
  printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"
	 "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
	 "********************************************************************",
	 "***                                                              ***",
	 "***        Welcome to 'Simpletron: The Computer Simulator'.      ***",
	 "***      Simpletron simulates a computer by running programs     ***",
	 "***         written in 'Simpletron Machine Language (SML),       ***",
	 "***         loading instructions into Simpletron 'memory'.       ***",
	 "***       You can enter programs manually with the keyboard,     ***",
	 "***            or by loading a program stored in a file.         ***",
	 "***                                                              ***",
	 "***        Mode           Description                            ***",
	 "***        ****           ***********                            ***",
	 "***         01  -  View list of saved programs,                  ***",
	 "***         02  -  Delete a program.                             ***",
	 "***         03  -  Load program (manually).                      ***",
	 "***         04  -  Load program (file).                          ***",
	 "***         05  -  Save program to file.                         ***",
	 "***         06  -  View loaded program instructions.             ***",
	 "***         07  -  Edit loaded program.                          ***",
	 "***         08  -  Run loaded program.                           ***",
	 "***         09  -  Print register.                               ***",
	 "***         10  -  Print Simpletron 'memory'.                    ***",
	 "***         11  -  Print Simpletron dump ('memory' & registers). ***",
	 "***         12  -  Save Simpletron dump to file.                 ***",
	 "***         13  -  Quit 'Simpletron: The Computer Simulator'.    ***",
	 "***                                                              ***",
	 "********************************************************************");
}

//  *****************************************************************************
//  ***                         Function 'print_goodbye'                      ***
//  *****************************************************************************
void print_goodbye(void)
{
  printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
	 "********************************************************************",
	 "***                                                              ***",
	 "***                     Thank you for using                      ***",
	 "***            'Simpletron: The Computer Simulator'.             ***",
	 "***                                                              ***",
	 "***                         Goodbye :-)                          ***",
	 "***                                                              ***",
	 "********************************************************************");
}

//  *****************************************************************************
//  ***                     Function 'print_entry_prompt'                     ***
//  *****************************************************************************
void print_entry_prompt(void)
{
  printf("\n%s\n%s\n%s\n%s\n%s\n%s%X%s\n%s\n%s\n\n",
	 "********************************************************************",
	 "***                                                              ***",
	 "***   Please enter your program one instruction (or data word)   ***",
	 "***   at a time. When you see the prompt '0-- +', please enter   ***",
	 "***         an instruction using an hexadecimal integer.         ***",
	 "***   Type the sentinel ",
	 SENTINEL,
	 " to stop entering your program.  ***",
	 "***                                                              ***",
	 "********************************************************************");
}

//  *****************************************************************************
//  ***                     Function 'enter_instruction'                      ***
//  *****************************************************************************
void enter_instruction(int *instruction_ptr, int *location_number_ptr,
		       int *instruction_counter_ptr)
{
  // Prompt the user to enter an instruction as a hexadecimal integer,
  // then increment 'instruction_counter_ptr'.
  printf("%25s%03d%s", "", *location_number_ptr, " +");
  scanf("%X", instruction_ptr);

  // Increment 'instruction_counter_ptr' unless sentinel entered.
  if (*instruction_ptr != SENTINEL) {
    (*instruction_counter_ptr)++;
  }
}

//  *****************************************************************************
//  ***                     Function 'invalid_entry'                          ***
//  *****************************************************************************
void invalid_entry(int *instruction_ptr, int *location_number_ptr,
		   int *instruction_counter_ptr)
{
  // If the instruction input is outside the range allowed by the program
  // (not including the sentinel value), print an appropriate error message
  // and prompt the user to re-enter an instruction (call enter_instruction).
  while ((*instruction_ptr < SENTINEL) || (SENTINEL > *instruction_ptr < -0XFFFF)
	 || (*instruction_ptr > 0XFFFF)) {
    printf("\n%s\n\n",
	   "XXX        "
	   " Invalid User Input. Enter valid instruction."
	   "         XXX");

    // Decrement 'instruction_counter_ptr' to subtract the invalid instruction
    // from the count.
    (*instruction_counter_ptr)--;

    // Prompt user to enter instruction.
    enter_instruction(instruction_ptr, location_number_ptr,
		      instruction_counter_ptr);
  }
}  

//  *****************************************************************************
//  ***                         Colour Functions                              ***
//  *****************************************************************************

// Begin function 'red'
void red(void)
{ 
  printf("\033[0;31m"); 
}

// Begin function 'blue'
void blue(void)
{
  printf("\033[0;34m"); 
}

// Begin function 'green'
void green(void)
{
  printf("\033[0;32m");
}

// Begin function 'yellow'
void yellow(void)
{
  printf("\033[0;33m");  
}

// Begin function 'reset'
void reset(void)
{
  printf("\033[0m");
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
