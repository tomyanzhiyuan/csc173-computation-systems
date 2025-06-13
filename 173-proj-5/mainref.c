/*
 * File: main.c
 * Creator: George Ferguson
 * Created: Mon Nov 28 14:11:17 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include "Circuit.h"

static Circuit and3_Circuit() {

    char *title = "Two AND gates connected to make a 3-input AND circuit";

    // Create the inputs
    int NINPUTS = 3;
    Boolean x = new_Boolean(false);
    Boolean y = new_Boolean(false);
    Boolean z = new_Boolean(false);
    Boolean* inputs = new_Boolean_array(NINPUTS);
    inputs[0] = x;
    inputs[1] = y;
    inputs[2] = z;

    // Create the outputs
    int NOUTPUTS = 1;
    Boolean result = new_Boolean(false);
    Boolean* outputs = new_Boolean_array(NOUTPUTS);
    outputs[0] = result;

    // Create the gates
    int NGATES = 2;
    Gate A1 = new_AndGate();
    Gate A2 = new_AndGate();
    Gate* gates = new_Gate_array(NGATES);
    gates[0] = A1;
    gates[1] = A2;

    // Create the circuit
    Circuit circuit = new_Circuit(title,
				  NINPUTS, inputs,
				  NOUTPUTS, outputs,
				  NGATES, gates);

    // Connect the gates in the circuit
    Circuit_connect(circuit, x, Gate_getInput(A1, 0));
    Circuit_connect(circuit, y, Gate_getInput(A1, 1));
    Boolean x_and_y = Gate_getOutput(A1);

    Circuit_connect(circuit, x_and_y, Gate_getInput(A2, 0));
    Circuit_connect(circuit, z, Gate_getInput(A2, 1));
    Boolean x_and_y_and_z = Gate_getOutput(A2);

    Circuit_connect(circuit, x_and_y_and_z, result);

    // Done!
    return circuit;
}

// You should not use this function in your project.
// It is here to show you how to get and set values in a circuit using the API.
// See the project description for what you are required to do.
// Go to study session if you have questions about how to do it.

// static void test3In1Out(Circuit circuit, bool in0, bool in1, bool in2) {
//     Circuit_setInput(circuit, 0, in0);
//     Circuit_setInput(circuit, 1, in1);
//     Circuit_setInput(circuit, 2, in2);
//     Circuit_update(circuit);
//     printf("%s %s %s -> %s\n",
// 	   Boolean_toString(Circuit_getInput(circuit, 0)),
// 	   Boolean_toString(Circuit_getInput(circuit, 1)),
// 	   Boolean_toString(Circuit_getInput(circuit, 2)),
// 	   Boolean_toString(Circuit_getOutput(circuit, 0)));
// }


// int main(int argc, char **argv) {
//     Circuit circuit = and3_Circuit();
//     Circuit_dump(circuit);
//     printf("\n");
//     printf("Testing: Some input(s) false: should be false\n");
//     test3In1Out(circuit, true, true, false);
//     printf("Testing: All inputs true: should be true\n");
//     test3In1Out(circuit, true, true, true);
//     printf("\nNote: Your program needs a function that tests ANY circuit on ALL possible\ncombinations of input values, in order from all false to all true, per the\nproject description.\n");
//     free_Circuit(circuit);
// }
