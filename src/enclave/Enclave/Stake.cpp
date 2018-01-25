#include "Stake.h"

#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void stake(uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  hello_world();
  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  while (r.has_next()) {
    const tuix::Row *row = r.next();
    w.write(row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();
}

