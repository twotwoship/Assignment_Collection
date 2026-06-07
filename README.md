# Stack Calculator

A calculator implementation project using the Stack data structure.

## Features

* Supports basic arithmetic operations (+, -, *, /)
* Supports parentheses
* Supports multi-digit numbers
* Converts infix expressions to postfix expressions
* Evaluates postfix expressions using a stack

## Data Structures Used

### Operator Stack

Used for handling operators and precedence during infix-to-postfix conversion.

### Number Stack

Used for evaluating postfix expressions and storing intermediate results.

## Program Workflow

1. Input an infix expression
2. Convert the infix expression to postfix notation
3. Evaluate the postfix expression using a stack
4. Output the final result

### Example

Input:

```text
(3+5)*2
```

Postfix Expression:

```text
3 5 + 2 *
```

Output:

```text
16
```

## Implementation Details

The program uses two separate stacks:

* Character Stack (`push_c`, `pop_c`, `peek_c`)

  * Stores operators and parentheses

* Integer Stack (`push_i`, `pop_i`, `peek_i`)

  * Stores operands and calculation results

## Learning Outcomes

Through this project, I learned:

* The practical application of the Stack data structure
* Operator precedence handling
* Infix-to-postfix expression conversion
* Postfix expression evaluation
* The relationship between data structures and algorithms

## Author

two_two_ship
