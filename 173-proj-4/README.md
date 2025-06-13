# CSC173 - project 4
##### name: tom yan zhiyuan
##### email: zyan17@u.rochester.edu
##### last updated: 14 November 2024

# Instructions
## Building the Project

To build the project, follow these steps:

1. Open a terminal and navigate to the project directory.
2. Run the following command:

```
gcc -std=c99 -Wall -Werror -g project4.c -o schema
```

This will compile the project and create an executable named `schema`.

## Running the Project

To run the project, use the following command in the terminal:

```
./schema
```

The program will start and prompt you for input based on the implemented parser.

## Memory Checking with Valgrind
To check for memory leaks and errors, you can use Valgrind:
```
valgrind --leak-check=full ./schema
```

This will run the program under Valgrind's supervision and report any memory issues.

Project freed all used memory by using free_tuple_list() and cleanup_database(). The former was used heavily for lookups.

```