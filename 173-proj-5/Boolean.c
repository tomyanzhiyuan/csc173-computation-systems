/*
 * Boolean.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "Boolean.h"

/**
 * A Boolean represents a mutable bool value.
 */
struct Boolean {
    bool value;
};

/**
 * Allocate and return a new Boolean with the given bool value.
 */
Boolean new_Boolean(bool value) {
    Boolean this = (Boolean)malloc(sizeof(struct Boolean));
    this->value = value;
    return this;
}

/**
 * Free the given Boolean.
 */
void free_Boolean(Boolean this) {
    free(this);
}

/**
 * Allocate and return an array of Booleans of the given length.
 */
Boolean* new_Boolean_array(int len) {
    // calloc(count, size)
    Boolean* this = (Boolean*)calloc(len, sizeof(Boolean));
    return this;
}

/**
 * Return the bool value of the given Boolean.
 */
bool Boolean_getValue(Boolean this) {
    return this->value;
}

/**
 * Set the value of the given Boolean to the given bool value.
 */
void Boolean_setValue(Boolean this, bool b) {
    this->value = b;
}

/**
 * Return the string representation of the value of the given Boolean.
 */
char* Boolean_toString(Boolean this) {
    //return this->value ? "T" : "F";
    return this->value ? "1" : "0";
}

/**
 * Print the the given Boolean (probably only useful for debugging).
 */
void Boolean_dump(Boolean this) {
    printf("%p=%s", this, Boolean_toString(this));
}
