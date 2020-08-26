//  *****************************************************************************
//  *
//  *    ex_12.c -- 
//  *    Author: Wade Shiell
//  *    Date Created: Sun Aug 23 17:02:54 2020
//  *
//  *****************************************************************************

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define EXPRESSION_LENGTH 40 // Maximum length of 'infix' and 'postfix'.

// Struct representing a node in a stack.
struct stack_node {
  char data; // Holds a character.
  struct stack_node *next_node_ptr; // Points to the next node in the stack.
};

typedef struct stack_node Stack_Node;
typedef Stack_Node *Stack_Node_Ptr;

// Function prototypes.
void convert_to_postfix(char infix[], char postfix[]);
int is_operator(char c);
int precedence(char operator1, char operator2);
void push(Stack_Node_Ptr *top_ptr, char value);
char pop(Stack_Node_Ptr *top_ptr);
char stack_top(Stack_Node_Ptr top_ptr);
int is_empty(Stack_Node_Ptr top_ptr);
void print_stack(Stack_Node_Ptr top_ptr);

int main(void)
{
  char infix[EXPRESSION_LENGTH]; // Holds an expression in infix form.
  char postfix[EXPRESSION_LENGTH]; // Holds an expression in postfix form.

  // Prompt user to enter an infix expression, and store it in 'infix'.
  puts("Enter expression in 'infix' form:");
  scanf("%s", infix);

  // Convert the infix expression entered into a postfix expression.
  convert_to_postfix(infix, postfix);
}

// Converts an 'infix' operation to a 'postfix' operation.
void convert_to_postfix(char infix[], char postfix[])
{
  Stack_Node_Ptr *top_node_ptr; // Points to the top of the stack.
  *top_node_ptr = NULL; // Initialise '*top_node_ptr'.
  unsigned int i = 0; // Current element index in 'infix'.
  unsigned int p = 0; // Current element index in 'postfix'.
  unsigned int length = strlen(infix); // Length of 'postfix'.
  char a; // Current character in 'infix'.

  push(top_node_ptr, '('); // Push '(' onto the stack.

  infix[length] = ')'; // Append ')' to 'infix'.
  /* infix[length + 1] = '\0'; */
  
  // While the stack is not empty, read 'infix' from left to right.
  while (!is_empty(*top_node_ptr)) {
    
    a = infix[i]; // Current character in 'infix'.
    
    // If the current character in 'infix' is a digit, copy it to the next
    // element of 'postfix'.
    if (isdigit(a)) {
      postfix[p] = a;
      p++;
    }
    // If the current character in 'infix' is '(', push it onto the stack.
    else if (a == '(') {
      push(top_node_ptr, a);
    }
    // If the current character in 'infix' is an operator,
    else if (is_operator(a)) {
      // Pop operator at the top of the stack while they have equal or
      // higher precedence than the current operator, and insert the
      // popped operators in 'postfix'.
      if (is_operator(stack_top(*top_node_ptr))) {
	if (precedence(a, (*top_node_ptr)->data) < 0) {
	  postfix[p] = pop(top_node_ptr);
	  p++;
	}
      }
      // Push the current character in 'infix' onto the stack.
      push(top_node_ptr, a);
    }
    // If the current character in 'infix' is ')',
    else if (a = ')') {
      // While the character at the top of the stack is not '(',
      // pop operators from the topc of the stack and insert them
      // in 'postfix'.
      while (stack_top(*top_node_ptr) != '(') {
	postfix[p] = pop(top_node_ptr);
	p++;
      }
      // Pop and discard '(' from the stack.
      pop(top_node_ptr);
    }
    else {
      // Not a valid character.
      printf("%s\n", "Invalid character.");
    }
    i++;
  }

  // Append '\0' to the end of 'postfix' and print.
  postfix[p] = '\0';
  printf("%s\n", postfix);
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

// Determines whether the precedence of operator1 is less than, equal to or
// greater than that of operator2. Returns -1, 0, 1 respectively.
int precedence(char operator1, char operator2)
{
  int a = 0; // Precedence value of operator1;
  int b = 0; // Precedence value of operator2;

  // Set precedence value = 0 if operator is '+' or '-'.
  if ((operator1 == '+') || (operator1 == '-')) {
    a = 0;      
  }
  if ((operator2 == '+') || (operator2 == '-')) {
    b = 0;      
  }
  // Set precedence value = 1 if operator is '*', '/' or '%'.
  if ((operator1 == '/') || (operator1 == '*') || (operator1 == '%')) {
    a = 1;      
  }
  if ((operator2 == '/') || (operator2 == '*') || (operator2 == '%')) {
    b = 1;      
  }
  // Set precedence value = 2 if operator is '^'.
  if (operator1 == '^') {
    a = 2;
  }
  if (operator2 == '^') {
    b = 2;
  }

  // Compare precedence values and return accordingly.
  if (a < b) {
    return -1;
  }
  else if (a == b) {
    return 0;
  }
  else {
    return 1;
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

// Returns the top value of the stack without popping the stack.
char stack_top(Stack_Node_Ptr top_ptr)
{
  char value; // Value in the top node.

  value = top_ptr->data;

  return value; // Return the value in the top node.
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
