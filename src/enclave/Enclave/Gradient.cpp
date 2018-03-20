#include "Gradient.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void lrgradient(uint8_t *regterm, size_t regterm_length,
            uint8_t *theta, size_t theta_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;

  flatbuffers::Verifier v1(regterm, regterm_length);
  check(v1.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Regularization Term %p of length %d\n", regterm, regterm_length);

  flatbuffers::Verifier v2(theta, theta_length);
  check(v2.VerifyBuffer<tuix::Row>(nullptr),
        "Corrupted Theta %p of length %d\n", theta, theta_length);

  const tuix::DoubleField* regularization_term = flatbuffers::GetRoot<tuix::DoubleField>(regterm);
  const double reg_value = regularization_term->value();
  double a = reg_value;
  a = a+1;

  double c_theta[ATTRIBUTE_BOUND];
  c_theta[0] = c_theta[0];

  const tuix::Row* para = flatbuffers::GetRoot<tuix::Row>(theta);
  int width = para->field_values()->Length();
  for(int i = 0; i < width; ++i){
    c_theta[i] = static_cast<const tuix::DoubleField*>(para->field_values()->Get(i)->value())->value(); 
  }

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double features[DATASET_BOUND];
  int sample_num = 0;
  int attribute_num = 0;
  double labels[SAMPLE_BOUND];
  double result[ATTRIBUTE_BOUND];

  int feature_ptr = 0;
  int label_ptr = 0;

  while(r.has_next()) {
    ++sample_num;
    const tuix::Row *row = r.next();
    //w.write(row);
    attribute_num = row->field_values()->Length()-1;
    for(int i = 0; i <= attribute_num; ++i) {
      double value = static_cast<const tuix::DoubleField*>(row->field_values()->Get(i)->value())->value();
      if(i != attribute_num)
        features[feature_ptr++] = value;
      else
        labels[label_ptr++] = value;
    }
  }
  features[0] = features[0];
  labels[0] = labels[0];

  //extract_dataset(r, features, labels, sample_num, attribute_num);

  lr_gradient(reg_value, c_theta, features, labels, sample_num, attribute_num, result);

  //serialize_vector(builder, w, result, 1, attribute_num);

  sample_num = 1;
  attribute_num = attribute_num;
  for(int i = 0; i < sample_num; ++i) {
    std::vector<flatbuffers::Offset<tuix::Field> > tmp_row;
    for(int j = 0; j < attribute_num; ++j) {
      tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, result[j]).Union()));
  }
    tmp_row.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, 0.0).Union()));
    const tuix::Row *final_row = flatbuffers::GetTemporaryPointer<tuix::Row>(builder, tuix::CreateRow(builder, builder.CreateVector(tmp_row)));
    w.write(final_row);
  }

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();
}

