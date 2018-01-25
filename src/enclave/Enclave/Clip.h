#include <cstddef>
#include <cstdint>

#ifndef CLIP_H
#define CLIP_H

/** Clip. */
void clip2norm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

void clipinfnorm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

#endif

