/*
 * Boolean.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef BOOLEAN_H_
#define BOOLEAN_H_

#include <stdbool.h>

/**
 * A Boolean represents a mutable bool value.
 */
typedef struct Boolean* Boolean;

/**
 * Allocate and return a new Boolean with the given bool value.
 */
extern Boolean new_Boolean(bool value);

/**
 * Free the given Boolean.
 */
extern void free_Boolean(Boolean this);

/**
 * Allocate and return an array of Booleans of the given length.
 */
extern Boolean* new_Boolean_array(int len);

/**
 * Return the bool value of the given Boolean.
 */
extern bool Boolean_getValue(Boolean this);

/**
 * Set the value of the given Boolean to the given bool value.
 */
extern void Boolean_setValue(Boolean this, bool b);

/**
 * Return the string representation of the value of the given Boolean.
 */
extern char* Boolean_toString(Boolean this);

/**
 * Print the the given Boolean (probably only useful for debugging).
 */
extern void Boolean_dump(Boolean this);

#endif /* BOOLEAN_H_ */

