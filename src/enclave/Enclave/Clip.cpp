#include "Clip.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

/*double FEATURES[1024];
double LABELS[128];
double RESULT[1024];*/

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
  double a = bound_value;
  a = a+1;

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double features[1024];
  int attribute_num;
  int sample_num; 
  double labels[128];
  double result[1024];

  extract_dataset(r, features, labels, attribute_num, sample_num);

  //passing(RESULT);
  clip_linf(bound_value, features, labels, attribute_num, sample_num, result);

  serialize_dataset(builder, w, result, labels, attribute_num, sample_num);
  /*for(int i = 0; i < sample_num; ++i) {
    std::vector<flatbuffers::Offset<tuix::Field>> tmp_row;
    for(int j = 0; j < attribute_num; ++j) {
      tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, features[i*attribute_num+i+j]).Union()));
    }
    tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, labels[i]).Union()));
    const tuix::Row *clipped_row = flatbuffers::GetTemporaryPointer<tuix::Row>(builder, tuix::CreateRow(builder, builder.CreateVector(tmp_row)));
    w.write(clipped_row);
  }*/
  /*while (r.has_next()) {
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
  }*/

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();

}

