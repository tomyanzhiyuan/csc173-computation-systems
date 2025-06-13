#
# File: Makefile
# Creator: George Ferguson
# Created: Mon Nov 28 13:52:02 2016
#

SRCS = Boolean.c Gates.c Circuit.c strdup.c circuit_test.c main.c
OBJS = $(SRCS:.c=.o)
EXE = boosim

#CFLAGS = -std=c99 -Wall -Werror -O0 -g
CFLAGS = -std=c99 -Wall -Werror -O0 -g -Wno-unused-function

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS)

valgrind:
	valgrind ./$(EXE)

# macOS
leaks: $(EXE)
	leaks -atExit -- ./$(EXE)

clean:
	-rm -r $(OBJS) $(EXE) $(EXE).dSYM

