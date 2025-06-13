# CSC173 - project 2
##### name: tom yan zhiyuan
##### email: zyan17@u.rochester.edu
##### last updated: 8 October 2024

# Instructions
## Building the Project

To build the project, follow these steps:

1. Open a terminal and navigate to the project directory.
2. Run the following command:

```
gcc -std=c99 -Wall -Werror -g project2.c -o parser
```

This will compile the project and create an executable named `parser`.

## Running the Project

To run the project, use the following command in the terminal:

```
./parser
```

The program will start and prompt you for input based on the implemented parser.

## Memory Checking with Valgrind

To check for memory leaks and errors, you can use Valgrind:

```
valgrind --leak-check=full ./parser
```

This will run the program under Valgrind's supervision and report any memory issues.



The final parsable grammar for our language is:
E → T U
U → +E | -E | ε
T → FG
G → *T | /T| ε
F → +F | -F | A
A → (E) | N
N → DM
M → N | ε
D → 0|1|2|3|4|5|6|7|8|9