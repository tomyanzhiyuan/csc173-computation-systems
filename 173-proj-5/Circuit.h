/*
 * Circuit.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include "Gates.h"

/**
 * A Circuit is a graph of Gates.
 */
typedef struct Circuit* Circuit;

/**
 * Allocate and return a new Circuit with the given title, inputs, outputs,
 * and gates.
 */
extern Circuit new_Circuit(char *title,
			   int numInputs, Boolean* inputs,
			   int numOutputs, Boolean* outputs,
			   int numGates, Gate* gates);

/**
 * Free the given Circuit, including its inputs, outputs, and gates.
 */
extern void free_Circuit(Circuit this);

/**
 * Return the title of the given Circuit.
 */
extern char* Circuit_getTitle(Circuit this);

/**
 * Return the number of inputs for the given Circuit.
 */
extern int Circuit_numInputs(Circuit this);

/**
 * Return the index'th Boolean input of the given Circuit.
 */
extern Boolean Circuit_getInput(Circuit this, int index);

/**
 * Set the value of the index'th Boolean input of the given Circuit to the
 * given bool value.
 */
extern void Circuit_setInput(Circuit this, int index, bool value);

/**
 * Return the number of outputs for the given Circuit.
 */
extern int Circuit_numOutputs(Circuit this);

/**
 * Return the index'th Boolean output of the given Circuit.
 */
extern Boolean Circuit_getOutput(Circuit this, int index);

/**
 * Return the number of Gates in the given Circuit.
 */
extern int Circuit_numGates(Circuit this);

/**
 * Return the index'th Gate in the given Circuit.
 */
extern Gate Circuit_getGate(Circuit this, int index);

/**
 * Print the given Circuit (probably only useful for debugging).
 */
extern void Circuit_dump(Circuit this);

/**
 * Connect the src Boolean to the dst Boolean in the given Circuit.
 */
extern void Circuit_connect(Circuit this, Boolean src, Boolean dst);

/**
 * Update all the values in this Circuit by updating the gates in topological
 * order, and propagating values from outputs to inputs based on the
 * Circuit's connections.
 */
extern void Circuit_update(Circuit this);

#endif /* CIRCUIT_H_ */
