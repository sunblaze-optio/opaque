#include "Stake.h"

#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void stake(uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  //test_dplr();
  flatbuffers::FlatBufferBuilder builder;

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double tmp[FEATURE_BOUND];
  while (r.has_next()) {
    const tuix::Row *row = r.next();
    int row_len = row->field_values()->Length();
    for(int i = 0; i < row_len; ++i) {
      tmp[i] = static_cast<const tuix::DoubleField*>(row->field_values()->Get(i)->value())->value();
    }
    std::vector<flatbuffers::Offset<tuix::Field> > tmp_row;
    for(int i = 0; i < row_len; ++i) {
      tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, tmp[i]).Union()));
    }
    const tuix::Row *final_row = flatbuffers::GetTemporaryPointer<tuix::Row>(builder, tuix::CreateRow(builder, builder.CreateVector(tmp_row)));
    w.write(final_row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();
}

