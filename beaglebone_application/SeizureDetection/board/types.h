/* Helper functions for implementing type conversions used in the "comm.h" and "uStim.h" libraries.
 *
 * Christos Strydis
 * Delft University of Technology
 * (C) 2011
 *
 */
#ifndef typeconversions_h
#define typeconversions_h

/* TODO: Check if string.h is needed */
#include <string.h>
#include <stdlib.h>

#include <inttypes.h>
#include "binary.h"

typedef uint8_t byte;

#define UPPER_BYTE(x) (uint8_t)(((x) >> 8) & 0x000000FF)
#define LOWER_BYTE(x) (uint8_t)((x) & 0x000000FF)

#endif
