#include "Clip.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void clip2norm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::Verifier v(bound, bound_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Bound %p of length %d\n", bound, bound_length);

  // const tuix::Bound* clip_bound = flatbuffers::GetRoot<tuix::Bound>(bound);
  // const double bound_value = clip_bound->value();

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  while (r.has_next()) {
    const tuix::Row *row = r.next();
    //whatever interface to clip the row
    const tuix::Row *clipped_row = row;
    //Some check operation here? See filter.
    w.write(clipped_row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();
}

void clipinfnorm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Verifier v(bound, bound_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Bound %p of length %d\n", bound, bound_length);

  const tuix::DoubleField* clip_bound = flatbuffers::GetRoot<tuix::DoubleField>(bound);
  const double bound_value = clip_bound->value();

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double features[1000000];
  int attribute_num;
  int sample_num; 
  double labels[10000];
  extract_dataset(r, features, labels, attribute_num, sample_num);

  clip_linf(bound_value, features, labels, attribute_num, sample_num, features);

  while (r.has_next()) {
    const tuix::Row *row = r.next();
    int row_len = row->field_values()->Length();
    std::vector<flatbuffers::Offset<tuix::Field>> tmp_row;
    for(int i = 0; i < row_len; ++i) {
      double value = static_cast<const tuix::DoubleField*>(row->field_values()->Get(i)->value())->value();
      if(value > bound_value) {
        tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, bound_value).Union()));
      }
      else {
        tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, value).Union()));
      }
    }
    const tuix::Row *clipped_row = flatbuffers::GetTemporaryPointer<tuix::Row>(builder, tuix::CreateRow(builder, builder.CreateVector(tmp_row)));
    w.write(clipped_row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();

}

