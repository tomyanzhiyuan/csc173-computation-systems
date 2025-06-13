# CSC173 - project 5
##### name: tom yan zhiyuan
##### email: zyan17@u.rochester.edu
##### last updated: 30 November 2024

To build the project, follow these steps:

1. Open a terminal and navigate to the project directory.
2. Run the following command:

# Compile using make
make

# Run the program
./boosim

## Memory Checking with Valgrind (with Docker)
To check for memory leaks and errors, you can use Valgrind:
```
1. Add executable permissions to the binary
chmod +x boosim

2. Run valgrind
valgrind --leak-check=full ./boosim
```

## Implementation of NAND and NOR Gates:
* The simulator provides only basic AND, OR, and NOT (Inverter) gates

* NAND gates are implemented using:
 - AND gate followed by NOT gate

* NOR gates are implemented using:
 - OR gate followed by NOT gate 