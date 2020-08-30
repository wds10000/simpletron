//  *****************************************************************************
//  *
//  *    infix_postfix.h -- Header file for 'infix_postfix.c'
//  *    Date Created: Sun Aug 30 15:49:44 2020
//  *
//  *****************************************************************************

#ifndef INFIX_POSTFIX_H
#define INFIX_POSTFIX_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct representing a node in a stack.
struct stack_node {
  char data; // Holds a character.
  struct stack_node *next_node_ptr; // Points to the next node in the stack.
};

typedef struct stack_node Stack_Node;
typedef Stack_Node *Stack_Node_Ptr;

// Converts an 'infix' operation to a 'postfix' operation.
void convert_to_postfix(char infix[], char postfix[]);

// Evaluate the postfix expression.
int evaluate_postfix_expression(char postfix[]);

// Evaluated the expression 'op1 operator op2'.
int calculate(int op1, int op2, char operator);

// Determines whether c is an operator.
int is_operator(char c);

// Determines whether the precedence of operator1 is less than, equal to or
// greater than that of operator2. Returns -1, 0, 1 respectively.
int precedence(char operator1, char operator2);

// Push a value onto the stack.
void push(Stack_Node_Ptr *top_ptr, char value);

// Pop a value from the stack.
char pop(Stack_Node_Ptr *top_ptr);

// Returns the top value of the stack without popping the stack.
char stack_top(Stack_Node_Ptr top_ptr);

// Determines whether the stack is empty.
int is_empty(Stack_Node_Ptr top_ptr);

// Prints the stack.
void print_stack(Stack_Node_Ptr top_ptr);

#endif

// ******************************************************************************
// **                                                                          **
// **                            END FILE                                      **
// **                                                                          **
// ******************************************************************************
