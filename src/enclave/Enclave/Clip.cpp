#include "Clip.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void clip2norm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Verifier v(bound, bound_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Bound %p of length %d\n", bound, bound_length);

  const tuix::DoubleField* clip_bound = flatbuffers::GetRoot<tuix::DoubleField>(bound);
  const double bound_value = clip_bound->value();
  double a  = bound_value;
  a = a+1;

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  while(r.has_next()){
    const tuix::Row *row = r.next();
    w.write(row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();
}

/*void clip2norm(uint8_t *bound, size_t bound_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Verifier v(bound, bound_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Bound %p of length %d\n", bound, bound_length);

  const tuix::DoubleField* clip_bound = flatbuffers::GetRoot<tuix::DoubleField>(bound);
  const double bound_value = clip_bound->value();
  double a = bound_value;
  a = a+1;

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double features[DATASET_BOUND];
  int sample_num;
  int attribute_num;
  double labels[SAMPLE_BOUND];
  double result[DATASET_BOUND];

  extract_dataset(r, features, labels, sample_num, attribute_num);

  //clip_l2(bound_value, features, labels, sample_num, attribute_num, result);

  serialize_dataset(builder, w, result, labels, sample_num, attribute_num);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();
}*/

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

  double features[DATASET_BOUND];
  int sample_num;
  int attribute_num; 
  double labels[SAMPLE_BOUND];
  double result[DATASET_BOUND];

  extract_dataset(r, features, labels, sample_num, attribute_num);

  clip_linf(bound_value, features, labels, sample_num, attribute_num, result);

  serialize_dataset(builder, w, result, labels, sample_num, attribute_num);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();

}

