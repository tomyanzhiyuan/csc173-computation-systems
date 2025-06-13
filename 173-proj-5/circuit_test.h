#ifndef CIRCUIT_TEST_H_
#define CIRCUIT_TEST_H_

#include "Circuit.h"

// Circuit testing functions
void set_inputs_from_binary(Circuit circuit, int input_val);
void test_circuit_all_combinations(Circuit circuit);

// Circuit creation functions
Circuit create_circuit_a(void);
Circuit create_circuit_b(void);
Circuit create_circuit_c(void);
Circuit create_extra_credit_circuit(void);  // Add this line


// Helper gates
Gate create_nand_gate(void);
Gate create_nor_gate(void);

#endif /* CIRCUIT_TEST_H_ */