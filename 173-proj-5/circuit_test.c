#include <stdio.h>
#include <stdlib.h>
#include "circuit_test.h"
#include "Boolean.h"
#include "Gates.h"
#include "Circuit.h"

/**
 * Set the circuit inputs based on a binary number representation.
 * @param circuit The circuit to set inputs for
 * @param input_val The binary number representing input values
 */
void set_inputs_from_binary(Circuit circuit, int input_val) {
    int num_inputs = Circuit_numInputs(circuit);
    for (int i = 0; i < num_inputs; i++) {
        // Extract bit at position i from right to left
        bool bit_val = (input_val >> i) & 1;
        Circuit_setInput(circuit, num_inputs - 1 - i, bit_val);
    }
}

/**
 * Test a circuit with all possible input combinations.
 * Prints results in binary order from all 0s to all 1s.
 */
void test_circuit_all_combinations(Circuit circuit) {
    int num_inputs = Circuit_numInputs(circuit);
    int num_outputs = Circuit_numOutputs(circuit);
    int total_combinations = 1 << num_inputs; // 2^num_inputs
    
    // Print header
    printf("\nTesting circuit: %s\n", Circuit_getTitle(circuit));
    printf("Inputs  Outputs\n");
    printf("------- -------\n");
    
    // Test each combination
    for (int i = 0; i < total_combinations; i++) {
        // Set inputs based on binary representation of i
        set_inputs_from_binary(circuit, i);
        
        // Update circuit to propagate values
        Circuit_update(circuit);
        
        // Print inputs
        for (int in = 0; in < num_inputs; in++) {
            printf("%s", Boolean_toString(Circuit_getInput(circuit, in)));
        }
        
        printf(" -> ");
        
        // Print outputs
        for (int out = 0; out < num_outputs; out++) {
            printf("%s", Boolean_toString(Circuit_getOutput(circuit, out)));
            if (out < num_outputs - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/**
 * Circuit (a) from the diagram
 * Two inputs (x,y), one output (result)
 */
Circuit create_circuit_a() {
    char *title = "Circuit A - Two inputs with NAND and OR feeding into AND";

    // Create inputs
    Boolean x = new_Boolean(false);
    Boolean y = new_Boolean(false);
    Boolean* inputs = new_Boolean_array(2);
    inputs[0] = x;
    inputs[1] = y;

    // Create output
    Boolean result = new_Boolean(false);
    Boolean* outputs = new_Boolean_array(1);
    outputs[0] = result;

    // Create gates
    Gate and1 = new_AndGate();  // First AND (part of NAND)
    Gate not1 = new_Inverter(); // NOT (completes NAND)
    Gate or1 = new_OrGate();    // OR gate for x and y
    Gate and2 = new_AndGate();  // Final AND gate
    
    Gate* gates = new_Gate_array(4);
    gates[0] = and1;
    gates[1] = not1;
    gates[2] = or1;
    gates[3] = and2;

    // Create circuit
    Circuit circuit = new_Circuit(title, 2, inputs, 1, outputs, 4, gates);

    // Create NAND gate (AND followed by NOT)
    Circuit_connect(circuit, x, Gate_getInput(and1, 0));
    Circuit_connect(circuit, y, Gate_getInput(and1, 1));
    Circuit_connect(circuit, Gate_getOutput(and1), Gate_getInput(not1, 0));

    // Create OR gate
    Circuit_connect(circuit, x, Gate_getInput(or1, 0));
    Circuit_connect(circuit, y, Gate_getInput(or1, 1));

    // Connect final AND gate with NAND and OR outputs
    Circuit_connect(circuit, Gate_getOutput(not1), Gate_getInput(and2, 0));  // NAND output
    Circuit_connect(circuit, Gate_getOutput(or1), Gate_getInput(and2, 1));   // OR output

    // Connect final output
    Circuit_connect(circuit, Gate_getOutput(and2), result);

    return circuit;
}

Circuit create_circuit_b() {
    char *title = "Circuit B - Three inputs (x,y,z) with AND, NAND, and OR";

    // Create inputs
    Boolean x = new_Boolean(false);
    Boolean y = new_Boolean(false);
    Boolean z = new_Boolean(false);
    Boolean* inputs = new_Boolean_array(3);
    inputs[0] = x;
    inputs[1] = y;
    inputs[2] = z;

    // Create output
    Boolean result = new_Boolean(false);
    Boolean* outputs = new_Boolean_array(1);
    outputs[0] = result;

    // Create gates
    Gate and1 = new_AndGate();  // AND gate for x and y
    Gate and2 = new_AndGate();  // AND part of NAND
    Gate not2 = new_Inverter(); // NOT part of NAND
    Gate or1 = new_OrGate();    // Final OR gate
    
    Gate* gates = new_Gate_array(4);
    gates[0] = and1;
    gates[1] = and2;
    gates[2] = not2;
    gates[3] = or1;

    // Create circuit
    Circuit circuit = new_Circuit(title, 3, inputs, 1, outputs, 4, gates);

    // Connect first AND gate (x and y)
    Circuit_connect(circuit, x, Gate_getInput(and1, 0));
    Circuit_connect(circuit, y, Gate_getInput(and1, 1));

    // Connect NAND gate (y and z)
    Circuit_connect(circuit, y, Gate_getInput(and2, 0));
    Circuit_connect(circuit, z, Gate_getInput(and2, 1));
    Circuit_connect(circuit, Gate_getOutput(and2), Gate_getInput(not2, 0));

    // Connect final OR gate with AND and NAND outputs
    Circuit_connect(circuit, Gate_getOutput(and1), Gate_getInput(or1, 0));   // AND output
    Circuit_connect(circuit, Gate_getOutput(not2), Gate_getInput(or1, 1));   // NAND output

    // Connect final output
    Circuit_connect(circuit, Gate_getOutput(or1), result);

    return circuit;
}

Circuit create_circuit_c() {
    char *title = "Circuit C - Three inputs (x,y,z) with NOT, AND, and OR combinations";

    // Create inputs
    Boolean x = new_Boolean(false);
    Boolean y = new_Boolean(false);
    Boolean z = new_Boolean(false);
    Boolean* inputs = new_Boolean_array(3);
    inputs[0] = x;
    inputs[1] = y;
    inputs[2] = z;

    // Create output
    Boolean result = new_Boolean(false);
    Boolean* outputs = new_Boolean_array(1);
    outputs[0] = result;

    // Create gates
    Gate not1 = new_Inverter();  // NOT for y
    Gate not2 = new_Inverter();  // NOT for z
    Gate not3 = new_Inverter();  // NOT for x
    Gate and1 = new_AndGate();   // AND for x and NOT y
    Gate and2 = new_AndGate();   // AND for y and NOT z
    Gate and3 = new_AndGate();   // AND for z and NOT x
    Gate or1 = new_OrGate();     // First OR gate
    Gate or2 = new_OrGate();     // Second OR gate (final)
    
    Gate* gates = new_Gate_array(8);
    gates[0] = not1;
    gates[1] = not2;
    gates[2] = not3;
    gates[3] = and1;
    gates[4] = and2;
    gates[5] = and3;
    gates[6] = or1;
    gates[7] = or2;

    // Create circuit
    Circuit circuit = new_Circuit(title, 3, inputs, 1, outputs, 8, gates);

    // Connect NOT gates
    Circuit_connect(circuit, y, Gate_getInput(not1, 0));  // NOT y
    Circuit_connect(circuit, z, Gate_getInput(not2, 0));  // NOT z
    Circuit_connect(circuit, x, Gate_getInput(not3, 0));  // NOT x

    // First AND gate (x and NOT y)
    Circuit_connect(circuit, x, Gate_getInput(and1, 0));
    Circuit_connect(circuit, Gate_getOutput(not1), Gate_getInput(and1, 1));

    // Second AND gate (y and NOT z)
    Circuit_connect(circuit, y, Gate_getInput(and2, 0));
    Circuit_connect(circuit, Gate_getOutput(not2), Gate_getInput(and2, 1));

    // Third AND gate (z and NOT x)
    Circuit_connect(circuit, z, Gate_getInput(and3, 0));
    Circuit_connect(circuit, Gate_getOutput(not3), Gate_getInput(and3, 1));

    // First OR gate (outputs of first and second AND gates)
    Circuit_connect(circuit, Gate_getOutput(and1), Gate_getInput(or1, 0));
    Circuit_connect(circuit, Gate_getOutput(and2), Gate_getInput(or1, 1));

    // Second OR gate (output of first OR and third AND)
    Circuit_connect(circuit, Gate_getOutput(or1), Gate_getInput(or2, 0));
    Circuit_connect(circuit, Gate_getOutput(and3), Gate_getInput(or2, 1));

    // Connect final output
    Circuit_connect(circuit, Gate_getOutput(or2), result);

    return circuit; 
}

Circuit create_extra_credit_circuit() {
    char *title = "Extra Credit - Functions f and g";

    // Create inputs
    Boolean a = new_Boolean(false);
    Boolean b = new_Boolean(false);
    Boolean c = new_Boolean(false);
    Boolean* inputs = new_Boolean_array(3);
    inputs[0] = a;
    inputs[1] = b;
    inputs[2] = c;

    // Create outputs (f and g)
    Boolean f = new_Boolean(false);
    Boolean g = new_Boolean(false);
    Boolean* outputs = new_Boolean_array(2);
    outputs[0] = f;  // Function f
    outputs[1] = g;  // Function g

    // Create NOT gates for first layer
    Gate not_a = new_Inverter();
    Gate not_b = new_Inverter();
    Gate not_c = new_Inverter();

    // Create AND gates for second layer (minterms)
    // For f: m3, m5, m6, m7
    Gate and_m3 = new_And3Gate();  // !a.b.c
    Gate and_m5 = new_And3Gate();  // a.!b.c
    Gate and_m6 = new_And3Gate();  // a.b.!c
    Gate and_m7 = new_And3Gate();  // a.b.c

    // For g: m1, m2, m4, m7
    Gate and_m1 = new_And3Gate();  // !a.!b.c
    Gate and_m2 = new_And3Gate();  // !a.b.!c
    Gate and_m4 = new_And3Gate();  // a.!b.!c

    // Create OR gates for third layer (outputs)
    Gate or_f = new_Or4Gate();  // Combines minterms for f
    Gate or_g = new_Or4Gate();  // Combines minterms for g

    // Create array of all gates
    Gate* gates = new_Gate_array(12);  // 3 NOTs + 8 ANDs + 2 ORs
    int gate_index = 0;
    gates[gate_index++] = not_a;
    gates[gate_index++] = not_b;
    gates[gate_index++] = not_c;
    gates[gate_index++] = and_m3;
    gates[gate_index++] = and_m5;
    gates[gate_index++] = and_m6;
    gates[gate_index++] = and_m7;
    gates[gate_index++] = and_m1;
    gates[gate_index++] = and_m2;
    gates[gate_index++] = and_m4;
    gates[gate_index++] = or_f;
    gates[gate_index++] = or_g;

    // Create circuit
    Circuit circuit = new_Circuit(title, 3, inputs, 2, outputs, 12, gates);

    // Layer 1: Connect inputs to NOT gates
    Circuit_connect(circuit, a, Gate_getInput(not_a, 0));
    Circuit_connect(circuit, b, Gate_getInput(not_b, 0));
    Circuit_connect(circuit, c, Gate_getInput(not_c, 0));

    // Layer 2: Connect to AND gates for f minterms
    // m3 = !a.b.c
    Circuit_connect(circuit, Gate_getOutput(not_a), Gate_getInput(and_m3, 0));
    Circuit_connect(circuit, b, Gate_getInput(and_m3, 1));
    Circuit_connect(circuit, c, Gate_getInput(and_m3, 2));

    // m5 = a.!b.c
    Circuit_connect(circuit, a, Gate_getInput(and_m5, 0));
    Circuit_connect(circuit, Gate_getOutput(not_b), Gate_getInput(and_m5, 1));
    Circuit_connect(circuit, c, Gate_getInput(and_m5, 2));

    // m6 = a.b.!c
    Circuit_connect(circuit, a, Gate_getInput(and_m6, 0));
    Circuit_connect(circuit, b, Gate_getInput(and_m6, 1));
    Circuit_connect(circuit, Gate_getOutput(not_c), Gate_getInput(and_m6, 2));

    // m7 = a.b.c
    Circuit_connect(circuit, a, Gate_getInput(and_m7, 0));
    Circuit_connect(circuit, b, Gate_getInput(and_m7, 1));
    Circuit_connect(circuit, c, Gate_getInput(and_m7, 2));

    // Layer 2: Connect to AND gates for g minterms
    // m1 = !a.!b.c
    Circuit_connect(circuit, Gate_getOutput(not_a), Gate_getInput(and_m1, 0));
    Circuit_connect(circuit, Gate_getOutput(not_b), Gate_getInput(and_m1, 1));
    Circuit_connect(circuit, c, Gate_getInput(and_m1, 2));

    // m2 = !a.b.!c
    Circuit_connect(circuit, Gate_getOutput(not_a), Gate_getInput(and_m2, 0));
    Circuit_connect(circuit, b, Gate_getInput(and_m2, 1));
    Circuit_connect(circuit, Gate_getOutput(not_c), Gate_getInput(and_m2, 2));

    // m4 = a.!b.!c
    Circuit_connect(circuit, a, Gate_getInput(and_m4, 0));
    Circuit_connect(circuit, Gate_getOutput(not_b), Gate_getInput(and_m4, 1));
    Circuit_connect(circuit, Gate_getOutput(not_c), Gate_getInput(and_m4, 2));

    // Layer 3: Connect to OR gates for outputs
    // Connect f minterms to OR gate
    Circuit_connect(circuit, Gate_getOutput(and_m3), Gate_getInput(or_f, 0));
    Circuit_connect(circuit, Gate_getOutput(and_m5), Gate_getInput(or_f, 1));
    Circuit_connect(circuit, Gate_getOutput(and_m6), Gate_getInput(or_f, 2));
    Circuit_connect(circuit, Gate_getOutput(and_m7), Gate_getInput(or_f, 3));

    // Connect g minterms to OR gate
    Circuit_connect(circuit, Gate_getOutput(and_m1), Gate_getInput(or_g, 0));
    Circuit_connect(circuit, Gate_getOutput(and_m2), Gate_getInput(or_g, 1));
    Circuit_connect(circuit, Gate_getOutput(and_m4), Gate_getInput(or_g, 2));
    Circuit_connect(circuit, Gate_getOutput(and_m7), Gate_getInput(or_g, 3));

    // Connect OR gates to outputs
    Circuit_connect(circuit, Gate_getOutput(or_f), f);
    Circuit_connect(circuit, Gate_getOutput(or_g), g);

    return circuit;
}