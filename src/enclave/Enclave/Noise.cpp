#include "Noise.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

void addlaplacenoise(uint8_t *noise_para, size_t noise_para_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Verifier v(noise_para, noise_para_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Noise Para %p of length %d\n", noise_para, noise_para_length);

  const tuix::DoubleField* para = flatbuffers::GetRoot<tuix::DoubleField>(noise_para);
  const double b = para->value();

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double theta[ATTRIBUTE_BOUND];
  int sample_num = 0; 
  int attribute_num = 0;
  double result[ATTRIBUTE_BOUND];

  extract_vector(r, theta, sample_num, attribute_num);

  add_laplace(b, theta, attribute_num, result);

  serialize_vector(builder, w, result, sample_num, attribute_num);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();
}

void addgaussiannoise(uint8_t *noise_para, size_t noise_para_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Verifier v(noise_para, noise_para_length);
  check(v.VerifyBuffer<tuix::DoubleField>(nullptr),
        "Corrupted Noise Para %p of length %d\n", noise_para, noise_para_length);

  const tuix::DoubleField* para = flatbuffers::GetRoot<tuix::DoubleField>(noise_para);
  const double std_dev = para->value();

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  double theta[ATTRIBUTE_BOUND];
  int sample_num = 0;
  int attribute_num = 0;
  double result[ATTRIBUTE_BOUND];

  extract_vector(r, theta, sample_num, attribute_num);

  add_gaussian(std_dev, theta, attribute_num, result);
  serialize_vector(builder, w, result, sample_num, attribute_num);

  w.finish(w.write_encrypted_blocks());
  *output_rows = w.output_buffer().release();
  *output_rows_length = w.output_size();
}
