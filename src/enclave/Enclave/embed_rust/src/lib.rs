#![feature(prelude_import)]
#![no_std]

#[macro_use]
extern crate sgx_tstd as std;
extern crate sgx_rand as rand;
extern crate ndarray;
extern crate ndarray_rand_sgx as ndarray_rand;
//extern crate rand;
//extern crate ndarray_linalg_sgx as ndarray_linalg;

#[allow(unused)]
#[prelude_import]
use std::prelude::v1::*;
use std::f64::consts::E;
use std::error::Error;
use ndarray::{Array1, Array2};
//use rand::distributions::normal::{Normal, IndependentSample};

mod mockup_linalg;

//mod clip;

mod gradient;
use gradient::lr_gradient;

use rand::distributions::Range;
use rand::distributions::Normal;
use ndarray::{Array, Ix2};
use ndarray_rand::RandomExt;

#[no_mangle]
pub extern "C" fn hello_world() {
   let x = 1.0_f64;
   let y = x.powf(2.3);
}

#[no_mangle]
pub extern "C" fn passing(arr: &mut [f64], len: i32) {
  arr[1] = 0.0;
}

#[no_mangle]
pub extern "C" fn logistic_regression(features: &Array2<f64>, labels: &Array2<f64>, lambda: f64, learning_rate: f64, eps: f64, delta: f64) -> Array2<f64> {
  let n = features.shape()[0] as f64;
  let m = features.shape()[1] as f64;
  let L: f64 = 1.0;
  let num_iters = 100 as usize;
  let mut theta = Array2::<f64>::zeros((features.shape()[1], 1));
  let std_dev: f64 = 4.0*L*((num_iters as f64)*(1.0/delta).ln()).sqrt()/(n*eps);
  for i in 1..num_iters { 
    let gradient = lr_gradient(features, labels, &theta, lambda);
    // is multiplication of scalar and vector supported in Rust?
    let noise = Array::random(gradient.shape(), (Normal::new(0., std_dev)));
    theta = theta - learning_rate * (gradient+noise);
  }
  return theta;
}
