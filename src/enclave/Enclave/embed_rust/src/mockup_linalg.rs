use std::error::Error;

use ndarray::{Array1, Array2};
//use ndarray_linalg::Inverse;

pub extern "C" fn transpose(x: &Array2<f64>) -> &Array2<f64> {
  /*let height = x.shape()[0] as i32;
  let width = x.shape()[1] as i32;
  for
    for
      tmp
      tmp = x[[i, j]]
      x[[i, j]] = x[[j, i]];
      x[[j,i]] = tmp;*/
  return x;
}

pub extern "C" fn pointwise_exp(x: &Array2<f64>) -> &Array2<f64> {
  /*let width = x.shape()[0] as i32;
  for
    x[i] = E.powf(x[i])*/
  return x;
}
