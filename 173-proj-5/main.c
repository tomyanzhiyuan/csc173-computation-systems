#include <stdio.h>
#include <stdlib.h>
#include "circuit_test.h"

int main(int argc, char **argv) {
    // Create and test Circuit A
    Circuit circuit_a = create_circuit_a();
    test_circuit_all_combinations(circuit_a);
    free_Circuit(circuit_a);

    // Create and test Circuit B
    Circuit circuit_b = create_circuit_b();
    test_circuit_all_combinations(circuit_b);
    free_Circuit(circuit_b);

    // Create and test Circuit C
    Circuit circuit_c = create_circuit_c();
    test_circuit_all_combinations(circuit_c);
    free_Circuit(circuit_c);

    // Test extra credit circuit
    printf("\nTesting Extra Credit Circuit\n");
    Circuit extra = create_extra_credit_circuit();
    test_circuit_all_combinations(extra);
    free_Circuit(extra);

    return 0;
}