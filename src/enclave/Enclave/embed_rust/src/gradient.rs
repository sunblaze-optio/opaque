use std::f64::consts::E;
use std::error::Error;
use ndarray::{Array1, Array2};
use mockup_linalg::{scalar_add, scalar_divide, pointwise_exp};

pub extern "C" fn lr_gradient(features: &Array2<f64>, labels: &Array2<f64>, theta: &Array2<f64>, lambda: f64) -> Array2<f64> {
  let height = features.shape()[0] as f64;
  let exponent = labels * &(features.dot(theta));
  let gradient_loss = scalar_divide(&(-((features.t()).dot(&(labels/&(scalar_add(&pointwise_exp(&exponent), 1.0_f64)))))), height);
  return gradient_loss;
}

/*pub extern "C" fn hubersvm_gradient() {
}*/
