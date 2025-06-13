# CSC173 - project 1
##### name: tom yan zhiyuan
##### email: zyan17@u.rochester.edu
##### last updated: 17 September 2024

# Instructions
## Building the Project

To build the project, follow these steps:

1. Open a terminal and navigate to the project directory.
2. Run the following command:

```
gcc -std=c99 -Wall -Werror -g project.c -o automata
```

This will compile the project and create an executable named `automata`.

## Running the Project

To run the project, use the following command in the terminal:

```
./automata
```

The program will start and prompt you for input based on the implemented automata.

## Memory Checking with Valgrind

To check for memory leaks and errors, you can use Valgrind:

```
valgrind --leak-check=full ./automata
```

This will run the program under Valgrind's supervision and report any memory issues.