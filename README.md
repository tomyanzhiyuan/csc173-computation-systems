# CSC173 - Computation and Formal Systems

This repository contains coursework and projects from **CSC173: Computation and Formal Systems** at the University of Rochester. The course explores computational theory, formal languages, parsing, functional programming, and digital circuit design.

## Course Overview

CSC173 covers fundamental concepts in theoretical computer science including:
- Finite automata and regular languages
- Context-free grammars and parsing
- Functional programming paradigms
- Database query languages and relational algebra
- Boolean algebra and digital circuit design

## Projects

### 📁 Project 1: Finite Automata
**Directory:** `173-proj-1/`  
**Language:** C  
**Focus:** Implementation of finite state automata and regular language recognition

- **File:** `project.c` - Core automata implementation
- **Executable:** `project` (compiled binary)
- **Build:** `gcc -std=c99 -Wall -Werror -g project.c -o automata`
- **Run:** `./automata`

### 📁 Project 2: Recursive Descent Parser
**Directory:** `173-proj-2/`  
**Language:** C  
**Focus:** Implementation of a recursive descent parser for arithmetic expressions

- **File:** `project2.c` - Parser implementation
- **Executable:** `parser` (compiled binary)
- **Grammar:** Supports arithmetic expressions with proper operator precedence
- **Build:** `gcc -std=c99 -Wall -Werror -g project2.c -o parser`
- **Run:** `./parser`

**Supported Grammar:**
```
E → T U
U → +E | -E | ε
T → FG
G → *T | /T| ε
F → +F | -F | A
A → (E) | N
N → DM
M → N | ε
D → 0|1|2|3|4|5|6|7|8|9
```

### 📁 Project 3: Functional Programming in Lisp
**Directory:** `173-proj-3/`  
**Language:** Lisp  
**Focus:** Functional programming concepts and recursive algorithms

- **Files:**
  - `project3.lisp` - Main project implementation
  - `list.lisp` - List manipulation functions
  - `math.lisp` - Mathematical functions including Fibonacci (tail-recursive)
  - `set.lisp` - Set operations
  - `testP3.lisp` - Test cases
- **Features:** Tail-recursive implementations, proper nil handling for invalid inputs

### 📁 Project 4: Database Query Language
**Directory:** `173-proj-4/`  
**Language:** C  
**Focus:** Implementation of a simple database query system

- **File:** `project4.c` - Database schema and query implementation
- **Executable:** `project4` (compiled binary)
- **Features:** Memory-safe implementation with proper cleanup
- **Build:** `gcc -std=c99 -Wall -Werror -g project4.c -o schema`
- **Run:** `./schema`

### 📁 Project 5: Boolean Circuit Simulator
**Directory:** `173-proj-5/`  
**Language:** C  
**Focus:** Digital circuit simulation with Boolean logic gates

- **Core Files:**
  - `main.c` - Main program entry point
  - `Circuit.c/.h` - Circuit structure and operations
  - `Gates.c/.h` - Logic gate implementations
  - `Boolean.c/.h` - Boolean value handling
- **Build System:** `Makefile` provided
- **Build:** `make`
- **Run:** `./boosim`
- **Features:**
  - Basic gates: AND, OR, NOT
  - Composite gates: NAND (AND + NOT), NOR (OR + NOT)
  - Topological sorting for circuit evaluation
  - Memory-safe implementation with valgrind-clean execution

## Building and Running

### Prerequisites
- GCC compiler with C99 support
- Make (for Project 5)
- Lisp interpreter (for Project 3)
- Valgrind (optional, for memory debugging)

### General Build Instructions
Each project includes specific build instructions in its README file. Most C projects follow the pattern:

```bash
cd 173-proj-X/
gcc -std=c99 -Wall -Werror -g [source_file].c -o [executable_name]
./[executable_name]
```

### Memory Testing
All C projects support Valgrind for memory leak detection:
```bash
valgrind --leak-check=full ./[executable_name]
```

## Author Information
- **Name:** Tom Yan Zhiyuan
- **Email:** zyan17@u.rochester.edu
- **Institution:** University of Rochester
- **Course:** CSC173 - Computation and Formal Systems

## Project Timeline
- **Project 1:** September 2024 - Finite Automata
- **Project 2:** October 2024 - Recursive Descent Parser  
- **Project 3:** October 2024 - Functional Programming in Lisp
- **Project 4:** November 2024 - Database Query Language
- **Project 5:** November 2024 - Boolean Circuit Simulator

## Key Learning Outcomes
- **Theoretical Foundations:** Understanding of computational models and formal languages
- **Parsing Techniques:** Implementation of recursive descent parsing algorithms
- **Functional Programming:** Mastery of Lisp and functional programming paradigms
- **Systems Programming:** Low-level C programming with proper memory management
- **Digital Design:** Boolean algebra and circuit simulation principles

## Notes
- All projects emphasize memory safety and proper resource management
- Code follows academic best practices with comprehensive error handling
- Each project builds upon concepts from previous assignments
- Implementations prioritize correctness and educational value over optimization

---
*This repository represents academic coursework completed as part of the Computer Science curriculum at the University of Rochester.*