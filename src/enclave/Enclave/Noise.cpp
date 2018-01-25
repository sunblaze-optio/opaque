#include "Noise.h"

//#include "MatrixOperation.h"
#include "ExpressionEvaluation.h"
#include "common.h"

using namespace edu::berkeley::cs::rise::opaque;

double rand_normal(double mean, double stddev)
{//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 0;//2.0*rand()/RAND_MAX - 1;
            y = 0;//2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}

void gaussiannoise(uint8_t *noise_para, size_t noise_para_length,
            uint8_t *shape, size_t shape_length,
            uint8_t *input_rows, size_t input_rows_length,
            uint8_t **output_rows, size_t *output_rows_length) {

  flatbuffers::Verifier v1(noise_para, noise_para_length);
  check(v1.VerifyBuffer<tuix::NoisePara>(nullptr),
        "Corrupted Bound %p of length %d\n", noise_para, noise_para_length);

  flatbuffers::Verifier v2(shape, shape_length);
  check(v2.VerifyBuffer<tuix::Shape>(nullptr),
        "Corrupted Bound %p of length %d\n", shape, shape_length);

  // const tuix::Bound* clip_bound = flatbuffers::GetRoot<tuix::Bound>(bound);
  // const double bound_value = clip_bound->value();

  EncryptedBlocksToRowReader r(input_rows, input_rows_length);
  FlatbuffersRowWriter w;

  /*std::default_random_engine generator;
  std::normal_distribution<double> distribution (0.0,1.0); 
  distribution(generator);*/

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

