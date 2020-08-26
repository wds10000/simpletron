//  *****************************************************************************
//  *
//  *    ex_14.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Sun Aug 23 17:02:54 2020
//  *
//  *****************************************************************************

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EXPRESSION_LENGTH 40 // Maximum length of 'infix' and 'postfix'.

// Struct representing a node in a stack.
struct stack_node {
  char data; // Holds a character.
  struct stack_node *next_node_ptr; // Points to the next node in the stack.
};

typedef struct number Number;
typedef Number *Number_Ptr;
typedef struct stack_node Stack_Node;
typedef Stack_Node *Stack_Node_Ptr;

int evaluate_postfix_expression(char *expression);
int calculate(int op1, int op2, char operator);
void push(Stack_Node_Ptr *top_ptr, char value);
char pop(Stack_Node_Ptr *top_ptr);
int is_operator(char c);
int is_empty(Stack_Node_Ptr top_ptr);
char stack_top(Stack_Node_Ptr top_ptr);
void print_stack(Stack_Node_Ptr top_ptr);

int main(void)
{
  char postfix[EXPRESSION_LENGTH]; // Array to hold 'postix' expression.

  puts("Enter a postfix expression (enter numbers in the form '--'):");
  scanf("%s", postfix);
  

  evaluate_postfix_expression(postfix);
}

// Evaluate the postfix expression.
int evaluate_postfix_expression(char *expression)
{
  Stack_Node_Ptr *top_node_ptr; // Points to the top of the stack.
  *top_node_ptr = NULL; // Initialise '*top_node_ptr'.

  unsigned int p = 0; // Current element index of 'expression'.
  int x, y; // Operand variables.
  int number = 0; // Holds the value of two-digit numbers;
  
  // While '\0' has not been encountered, read 'expression' from left to right.
  while (expression[p] != '\0'){

    // If the current character is a digit, push it onto the stack.
    if (isdigit(expression[p])) {
      number = (expression[p] - '0') * 10 + (expression[p + 1] - '0');
      push(top_node_ptr, number + '0');
      p++;
    }
    // If the current character is an operator, pop the top two elements of the
    // stack and assign them to variables x and y. Calculate x and y.
    else if (is_operator(expression[p])) {
      printf("%d\n", stack_top(*top_node_ptr) - '0');
      x = pop(top_node_ptr) - '0';
      printf("%d\n", stack_top(*top_node_ptr) - '0');
      y = pop(top_node_ptr) - '0';
      printf("%c\n", expression[p]);
      printf("%d\n\n", calculate(x, y, expression[p]));      
      push(top_node_ptr, calculate(x, y, expression[p]) + '0');
    }
    // Print message if character read is invalid.
    else {
      printf("%s\n", "Invalid character.");
    }
    p++;
  }
  
  // Print the result of the 'postfix' evaluation.
  printf("%s%d\n", "The result of evaluating the expression is: ",
	 pop(top_node_ptr) - '0');
}

// Evaluated the expression 'op1 operator op2'.
int calculate(int op1, int op2, char operator)
{
  switch (operator) {
  case '+':
    return op2 + op1;
  case '-':
    return op2 - op1;
  case '*':
    return op2 * op1;
  case '/':
    return op2 / op1;
  case '%':
    return op2 % op1;
  case '^':
    return pow(op2, op1);
  default:
    break;
  }
}

// Determines whether c is an operator.
int is_operator(char c)
{
  // Compare 'c' to each of the relevant operators. If there is a match,
  // return '1', otherwise return 0.
  switch (c) {

  case '+':
    return 1;
    break;
  case '-':
    return 1;
    break;
  case '*':
    return 1;
    break;
  case '/':
    return 1;
    break;
  case '^':
    return 1;
    break;
  case '%':
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

// Push a value onto the stack.
void push(Stack_Node_Ptr *top_ptr, char value)
{
  Stack_Node_Ptr new_node_ptr; // Declare a pointer to a node.

  // Use malloc to allocate memory to the new node pointer.
  new_node_ptr = malloc(sizeof(Stack_Node));

  // If memory is assigned successfully, assign 'value' to member 'data' and
  // assign the pointer to the previous top node to it's pointer member.
  if (new_node_ptr != NULL){
    new_node_ptr->data = value;
    new_node_ptr->next_node_ptr = *top_ptr;

    // Assign 'top_ptr' to the new node, which is at the top of the stack.
    *top_ptr = new_node_ptr;
  }
  // If memory was not allocated successfully, print error message.
  else {
    printf("%s/n", "Memory was not allocated.");
  }
}

// Pop a value from the stack.
char pop(Stack_Node_Ptr *top_ptr)
{
  char value; // Value to be returned.
  // Assign the pointer to the top node to 'temp_ptr'.
  Stack_Node_Ptr temp_ptr = *top_ptr; 

  // Assign the value in the top node to 'value'.
  value = (*top_ptr)->data;
  // Point 'top_ptr' at the next node in the stack.
  *top_ptr = (*top_ptr)->next_node_ptr;

  // Free the memory associated with the top node.
  free(temp_ptr);

  // Return the value in the popped node.
  return value;
}

// Determines whether the stack is empty.
int is_empty(Stack_Node_Ptr top_ptr)
{
  // If the top node is NULL, the stack is empty, so return 1.
  // Otherwise, return 0.
  if (top_ptr == NULL) {
    return 1;
  }
  else {
    return 0;
  }
}

// Returns the top value of the stack without popping the stack.
char stack_top(Stack_Node_Ptr top_ptr)
{
  char value; // Value in the top node.

  value = top_ptr->data;

  return value; // Return the value in the top node.
}

// Prints the stack.
void print_stack(Stack_Node_Ptr top_ptr)
{
  // If the stack is not empty, print it's values. Otherwise, print an
  // appropriate message.
  if  (!is_empty(top_ptr)) {
  // Iterate through the stack, printing the value in each node until there
  // are no more nodes left.
    while (top_ptr != NULL) {
      printf("%-2c", top_ptr->data);
      top_ptr = top_ptr->next_node_ptr;
    }
    puts("");
  }
  else {
    printf("%s\n", "Stack is empty.");
  }
}

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
