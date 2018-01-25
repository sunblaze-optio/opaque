#include <cstddef>
#include <cstdint>
#include "math.h"
#include "string.h"

#ifndef GRADIENT_H
#define GRADIENT_H

/** Gradient. */
void lrgradient(uint8_t *regterm, size_t regterm_length,
            uint8_t *theta, size_t theta_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

#endif
