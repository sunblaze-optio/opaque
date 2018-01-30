#![feature(prelude_import)]
#![no_std]

#[macro_use]
extern crate sgx_tstd as std;
extern crate ndarray;
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

//mod noise;
//use noise::gaussian_noise;

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
pub extern "C" fn logistic_regression(features: &Array2<f64>, labels: &Array2<f64>, lambda: f64, learning_rate: f64) -> Array2<f64> {
  let mut theta = Array2::<f64>::zeros((features.shape()[1], 1));
  for i in 1..100 { 
    let gradient = lr_gradient(features, labels, &theta, lambda);
    // is multiplication of scalar and vector supported in Rust?
    theta = theta - learning_rate * gradient;
  }
  return theta;
}
