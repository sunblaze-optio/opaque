#include <cstddef>
#include <cstdint>
#include "math.h"

#ifndef NOISE_H
#define NOISE_H

/** Noise. */
void gaussiannoise(uint8_t *noise_para, size_t noise_para_length,
            uint8_t *shape, size_t shape_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

#endif

