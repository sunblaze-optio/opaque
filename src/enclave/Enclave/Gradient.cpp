#include "Gradient.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void lr_gradient(uint8_t *regterm, size_t regterm_length,
            uint8_t *theta, size_t theta_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;

  flatbuffers::Verifier v1(regterm, regterm_length);
  check(v1.VerifyBuffer<tuix::RegularizationTerm>(nullptr),
        "Corrupted Bound %p of length %d\n", regterm, regterm_length);

  flatbuffers::Verifier v2(theta, theta_length);
  check(v2.VerifyBuffer<tuix::Parameters>(nullptr),
        "Corrupted Bound %p of length %d\n", theta, theta_length);

  const tuix::DoubleField* regularization_term = flatbuffers::GetRoot<tuix::DoubleField>(regterm);
  const double reg_value = regularization_term->value();

  double c_theta[128];

  const tuix::Row* para = flatbuffers::GetRoot<tuix::Row>(theta);
  int width = para->field_values()->Length();
  std::vector<double> para;
  for(int i = 0; i < width; ++i){
    c_theta[i] = static_cast<const tuix::DoubleField*>(para->field_values()->Get(i)->value()); 
  }

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double features[1024];
  int sample_num;
  int attribute_num;
  double labels[1024];
  double result[1024];

  extract_dataset(r, features, labels, sample_num, attribute_num);

  lr_gradient(reg_value, c_theta, features, labels, sample_num, attribute_num, result);

  serialize_vector(builder, w, result, attribute_num);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();
}

