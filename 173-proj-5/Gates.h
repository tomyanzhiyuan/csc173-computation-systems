/*
 * Gates.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef GATES_H_
#define GATES_H_

#include "Boolean.h"

/**
 * A Gate has one or more Boolean inputs and a single Boolean output,
 * and can update its output based on the values of its inputs.
 */
typedef struct Gate* Gate;

// Constructors only for concrete types of Gates (see below)

/**
 * Free the given Gate, including its input and output Booleans.
 */
extern void free_Gate(Gate this);

/**
 * Allocate and return an new array of Gates of the given length.
 */
extern Gate* new_Gate_array(int len);

/**
 * Run the given Gate's update function to update the value of its output
 * based on the value(s) of its input(s).
 */
extern void Gate_update(Gate this);

/**
 * Print the given Gate, including the values of its input(s) and output.
 * Probably only useful for debugging.
 */
extern void Gate_dump(Gate this);

/**
 * Return the number of inputs for the given Gate.
 */
extern int Gate_numInputs(Gate this);

 /**
 * Return the index'th input of the given Gate.
 */
extern Boolean Gate_getInput(Gate this, int index);

/**
 * Return the output of the given Gate.
 */
extern Boolean Gate_getOutput(Gate this);

//
// Concrete gate types implemented by this package:
//

/**
 * Return a new unary NOT Gate (an interverter).
 */
extern Gate new_Inverter();

/**
 * Return a new binary AND Gate.
 */
extern Gate new_AndGate();

/**
 * Return a new binary OR Gate.
 */
extern Gate new_OrGate();

/**
 * Return a new 3-input AND Gate.
 */
extern Gate new_And3Gate();

/**
 * Return a new 4-input OR Gate.
 */
extern Gate new_Or4Gate();

#endif /* GATES_H_ */
