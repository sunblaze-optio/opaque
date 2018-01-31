use rand::distributions::Range;
use rand::distributions::Normal;
use ndarray::{Array, Ix2};
use ndarray_rand::RandomExt;
use ndarray_rand::F32;

pub extern "C" fn gaussian_noise(length: i32, stddev: f64) {
  /*let normal = Normal::new(0.0, stddev);
  let result &mut [f64]
  for
    result[] = normal.ind_sample(&mut rand::thread_rng());
  pack result up as result_array;
  return result_array;*/
}
