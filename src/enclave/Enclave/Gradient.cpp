#include "Gradient.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void mtraverse(std::vector<std::vector<double> > &matrix, int height, int width, std::vector<std::vector<double> > &result) {
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j){
      result[j][i] = matrix[i][j];
    }
  }
}

void mvmultiply(std::vector<std::vector<double> > &matrix, std::vector<double> &vector, int height, int width, std::vector<double> &result) {
  for(int i = 0; i < height; ++i) {
      result[i] = 0; 
  }  
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      result[i] += matrix[i][j]*vector[j]; 
    } 
  }
} 

void vvpointmultiply(std::vector<double> &v1, std::vector<double> &v2, int width, std::vector<double> &result) {
  for(int i = 0; i < width; ++i){
    result[i] = v1[i]*v2[i];
  }
}

void vvpointdivide(std::vector<double> &v1, std::vector<double> &v2, int width, std::vector<double> &result){
  for(int i = 0; i < width; ++i) {
    result[i] = v1[i]/v2[i];
  }
}

void lr_gradient(std::vector<std::vector<double> > &features, std::vector<double> &labels, std::vector<double> &theta, int height, int width, std::vector<double> &result) {
  std::vector<double> tmp1(height, 0);
  mvmultiply(features, theta, height, width, tmp1);
  std::vector<double> tmp2(height, 0);
  vvpointmultiply(labels, tmp1, height, tmp2);
  for(int i = 0; i < height; ++i) {
    tmp1[i] = 1+exp(tmp2[i]);
  }
  vvpointdivide(labels, tmp1, height, tmp2);
  std::vector<std::vector<double> > tmp3(width, std::vector<double>(height, 0));
  mtraverse(features, height, width, tmp3);
  std::vector<double> tmp4(width, 0);
  mvmultiply(tmp3, tmp2, width, height, tmp4);
  for(int i = 0; i < width; ++i){
    result[i] = -tmp4[i]/height;
  }
}

void lrgradient(uint8_t *regterm, size_t regterm_length,
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

  //const tuix::RegularizationTerm* regularization_term = flatbuffers::GetRoot<tuix::RegularizationTerm>(regterm);
  //const double reg_value = regularization_term->value();

  const tuix::Parameters* para_tuix = flatbuffers::GetRoot<tuix::Parameters>(theta);
  int height = 0;
  int width = para_tuix->value()->Length();
  std::vector<double> para;
  for(int i = 0; i < width; ++i){
    para.push_back(para_tuix->value()->Get(i)->value());
  }

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  std::vector<std::vector<double>> features;
  std::vector<double> labels;

  while (r.has_next()) {
    ++height;
    const tuix::Row *row = r.next();
    int row_len = row->field_values()->Length();
    std::vector<double> tmp_row;
    for(int i = 0; i < row_len-1; ++i) {
      double value = static_cast<const tuix::DoubleField*>(row->field_values()->Get(i)->value())->value();
      tmp_row.push_back(value);
    }
    features.push_back(tmp_row);
    double label = static_cast<const tuix::DoubleField*>(row->field_values()->Get(row_len-1)->value())->value();
    labels.push_back(label);
  }

  std::vector<double> result(width, 0);
  lr_gradient(features, labels, para, height, width, result);

  std::vector<flatbuffers::Offset<tuix::Field> > field_result;
  for(int i = 0; i < width; ++i){
    field_result.push_back(tuix::CreateField(builder, tuix::FieldUnion_DoubleField, tuix::CreateDoubleField(builder, result[i]).Union()));   
  }

  const tuix::Row *final_result = flatbuffers::GetTemporaryPointer<tuix::Row>(builder, tuix::CreateRow(builder, builder.CreateVector(field_result)));
  w.write(final_result);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer();
  *output_rows_length = w.output_size();
}

