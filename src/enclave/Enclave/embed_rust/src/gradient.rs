use std::f64::consts::E;
use std::error::Error;
use ndarray::{Array1, Array2};
use mockup_linalg::{transpose, pointwise_exp};

pub extern "C" fn lr_gradient(features: &Array2<f64>, labels: &Array2<f64>, theta: &Array2<f64>, lambda: f64) -> Array2<f64> {
  let height = features.shape()[0];
  let width = features.shape()[1];
  let exponent = labels * &(features.dot(theta));
  let gradient_loss = -((features.t()).dot(&(labels/(pointwise_exp(&exponent)))));
  return gradient_loss;
}

/*pub extern "C" fn hubersvm_gradient() {
}*/
