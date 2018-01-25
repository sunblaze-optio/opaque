#include <cstddef>
#include <cstdint>

extern "C" {
  void hello_world();
}

#ifndef STAKE_H
#define STAKE_H

/** Stake. */
void stake(uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length);

#endif
