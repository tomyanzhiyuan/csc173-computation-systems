/*
 * File: Gates.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include "Gates.h"

/**
 * A Gate has one or more Boolean inputs and a single Boolean output,
 * and can update its output based on the values of its inputs.
 */
struct Gate {
    int numInputs;
    Boolean *inputs;
    Boolean output;
    void (*update)(Gate);
    void (*dump)(Gate);
};

/**
 * Allocate and return a new Gate (with no inputs and a single output).
 * This function is private for use by the (public) functions that create
 * specific types of gates (``subclasses'' of the ``abstract'' Gate).
 */
static Gate new_Gate() {
    Gate this = (Gate)malloc(sizeof(struct Gate));
    this->numInputs = 0;
    this->inputs = NULL;
    this->output = new_Boolean(false);
    return this;
}

/**
 * Free the given Gate, including its input and output Booleans.
 */
void free_Gate(Gate this) {
    if (this->inputs != NULL) {
	for (int i=0; i < this->numInputs; i++) {
	    free_Boolean(this->inputs[i]);
	}
	free(this->inputs);
    }
    free_Boolean(this->output);
    free(this);
}

/**
 * Allocate and return a new array of Gates of the given length.
 */
Gate* new_Gate_array(int len) {
    // calloc(count, size)
    Gate* this = (Gate*)calloc(len, sizeof(Gate));
    return this;
}

/**
 * Run the given Gate's update function to update the value of its output
 * based on the value(s) of its input(s).
 */
void Gate_update(Gate this) {
    this->update(this);
}

/**
 * Print the given Gate, including the values of its input(s) and output.
 * Probably only useful for debugging.
 */
void Gate_dump(Gate this) {
    this->dump(this);
}

/**
 * Return the number of inputs for the given Gate.
 */
int Gate_numInputs(Gate this) {
    return this->numInputs;
}

/**
 * Return the index'th input of the given Gate.
 */
Boolean Gate_getInput(Gate this, int index) {
    return this->inputs[index];
}

/**
 * Return the output of the given Gate.
 */
Boolean Gate_getOutput(Gate this) {
    return this->output;
}

/**
 * UnaryGates have a single input (and a single output).
 */
static Gate new_UnaryGate() {
    Gate this = new_Gate();
    this->numInputs = 1;
    this->inputs = new_Boolean_array(this->numInputs);
    return this;
}

static void UnaryGate_dump(Gate this, char* op) {
    printf("%s(", op);
    Boolean_dump(this->inputs[0]);
    printf(") -> ");
    Boolean_dump(this->output);
}

/**
 * BinaryGates have two inputs (and a single output).
 */
static Gate new_BinaryGate() {
    Gate this = new_Gate();
    this->numInputs = 2;
    this->inputs = new_Boolean_array(this->numInputs);
    return this;
}

static void BinaryGate_dump(Gate this, char *op) {
    printf("%s(", op);
    Boolean_dump(this->inputs[0]);
    printf(",");
    Boolean_dump(this->inputs[1]);
    printf(") -> ");
    Boolean_dump(this->output);
}

//
// Inverter
//

static void Inverter_update(Gate this) {
    Boolean_setValue(this->output, !Boolean_getValue(this->inputs[0]));
}

static void Inverter_dump(Gate this) {
    UnaryGate_dump(this, "NOT");
}

/**
 * Return a new unary NOT Gate (a.k.a. inverter).
 */
Gate new_Inverter() {
    Gate this = new_UnaryGate();
    this->inputs[0] = new_Boolean(false);
    this->update = Inverter_update;
    this->dump = Inverter_dump;
    return this;
}

//
// AndGate
//

static void AndGate_update(Gate this) {
    Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) && Boolean_getValue(this->inputs[1]));
}

static void AndGate_dump(Gate this) {
    BinaryGate_dump(this, "AND");
}

/**
 * Return a new binary AND Gate.
 */
Gate new_AndGate() {
    Gate this = new_BinaryGate();
    this->inputs[0] = new_Boolean(false);
    this->inputs[1] = new_Boolean(false);
    this->update = AndGate_update;
    this->dump = AndGate_dump;
    return this;
}

//
// OrGate
//

static void OrGate_update(Gate this) {
    Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) || Boolean_getValue(this->inputs[1]));
}

static void OrGate_dump(Gate this) {
    BinaryGate_dump(this, "OR");
}

/**
 * Return a new binary OR Gate.
 */
Gate new_OrGate() {
    Gate this = new_BinaryGate();
    this->inputs[0] = new_Boolean(false);
    this->inputs[1] = new_Boolean(false);
    this->update = OrGate_update;
    this->dump = OrGate_dump;
    return this;
}

//
// And3Gate
//

static void And3Gate_update(Gate this) {
    Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) && Boolean_getValue(this->inputs[1]) && Boolean_getValue(this->inputs[2]));
}

static void And3Gate_dump(Gate this) {
    printf("AND(");
    Boolean_dump(this->inputs[0]);
    printf(",");
    Boolean_dump(this->inputs[1]);
    printf(",");
    Boolean_dump(this->inputs[2]);
    printf(") -> ");
    Boolean_dump(this->output);
}

/**
 * Return a new 3-input AND Gate.
 */
Gate new_And3Gate() {
    Gate this = new_Gate();
    this->numInputs = 3;
    this->inputs = new_Boolean_array(this->numInputs);
    this->inputs[0] = new_Boolean(false);
    this->inputs[1] = new_Boolean(false);
    this->inputs[2] = new_Boolean(false);
    this->update = And3Gate_update;
    this->dump = And3Gate_dump;
    return this;
}

//
// Or4Gate
//

static void Or4Gate_update(Gate this) {
    Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) || Boolean_getValue(this->inputs[1]) || Boolean_getValue(this->inputs[2]) || Boolean_getValue(this->inputs[3]));
}

static void Or4Gate_dump(Gate this) {
    printf("OR(");
    Boolean_dump(this->inputs[0]);
    printf(",");
    Boolean_dump(this->inputs[1]);
    printf(",");
    Boolean_dump(this->inputs[2]);
    printf(",");
    Boolean_dump(this->inputs[3]);
    printf(") -> ");
    Boolean_dump(this->output);
}

/**
 * Return a new 4-input OR Gate.
 */
Gate new_Or4Gate() {
    Gate this = new_Gate();
    this->numInputs = 4;
    this->inputs = new_Boolean_array(this->numInputs);
    this->inputs[0] = new_Boolean(false);
    this->inputs[1] = new_Boolean(false);
    this->inputs[2] = new_Boolean(false);
    this->inputs[3] = new_Boolean(false);
    this->update = Or4Gate_update;
    this->dump = Or4Gate_dump;
    return this;
}

