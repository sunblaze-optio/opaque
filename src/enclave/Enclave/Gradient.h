#include <cstddef>
#include <cstdint>
#include "math.h"
#include "string.h"

#ifndef GRADIENT_H
#define GRADIENT_H

extern "C" {
  void lr_gradient(double, double*, double*, double*, int, int, double*);
}

/** Gradient. */
void lrgradient(uint8_t *regterm, size_t regterm_length,
            uint8_t *theta, size_t theta_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

#endif
