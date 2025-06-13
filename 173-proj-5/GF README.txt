
File: README.txt
Creator: George Ferguson

Simple Boolean circuit simulator written in C.

I had to replace my earlier reference-sharing implementation of circuit connections with an explicit list in order to support clean freeing of circuits and their components.

Replaced topological sort with a correct implementation (Dec 2022).

This code gets a clean report from valgrind when compiled with gcc on Fedora Linux (July 2017; also Dec 2022).

