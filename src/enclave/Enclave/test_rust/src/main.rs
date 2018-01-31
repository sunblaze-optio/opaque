extern crate rand;
extern crate ndarray;
extern crate ndarray_rand;

use std::f64::consts::E;
use ndarray::{Array, Array2, Ix2};
use rand::distributions::Normal;
use rand::distributions::Range;
use ndarray_rand::RandomExt;

/*mod mockup_linalg;

mod gradient;
use gradient::lr_gradient;*/

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
pub extern "C" fn print_array(x: &Array2<f64>) {
  for v in x.iter() {
    println!("{}", v);
  }
}

#[no_mangle]
pub extern "C" fn scalar_add(x: &Array2<f64>, s: f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()+s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}


#[no_mangle]
pub extern "C" fn scalar_subtract(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()-s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

#[no_mangle]
pub extern "C" fn scalar_subtracted(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| s-i.clone()).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}


#[no_mangle]
pub extern "C" fn scalar_multiply(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()*s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

#[no_mangle]
pub extern "C" fn scalar_divide(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()/s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

#[no_mangle]
pub extern "C" fn scalar_divided(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| s/i.clone()).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

#[no_mangle]
pub extern "C" fn pointwise_exp(x: &Array2<f64>) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| E.powf(i.clone())).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

#[no_mangle]
pub extern "C" fn lr_gradient(features: &Array2<f64>, labels: &Array2<f64>, theta: &Array2<f64>, lambda: f64) -> Array2<f64> {
  let height = features.shape()[0] as f64;
  let exponent = labels * &(features.dot(theta));
  let gradient_loss = scalar_divide(&(-((features.t()).dot(&(labels/&(scalar_add(&pointwise_exp(&exponent), 1.0_f64)))))), height);
  let regularization = scalar_multiply(theta, lambda);
  let result = gradient_loss + regularization;
  return result;
}

#[no_mangle]
pub extern "C" fn dp_logistic_regression(features: &Array2<f64>, labels: &Array2<f64>, lambda: f64, learning_rate: f64, eps: f64, delta: f64) -> Array2<f64> {
  let n = features.shape()[0] as f64;
  let l: f64 = 1.0;
  let num_iters = 1000 as usize;
  let mut theta = Array2::<f64>::zeros((features.shape()[1], 1));
  let std_dev: f64 = 4.0*l*((num_iters as f64)*(1.0/delta).ln()).sqrt()/(n*eps);
  for i in 1..num_iters { 
    let gradient = lr_gradient(features, labels, &theta, lambda);
    let noise = Array::random(gradient.shape(), (Normal::new(0., std_dev)));
    theta = theta - learning_rate * (gradient+noise);
  }
  return theta;
}

#[no_mangle]
pub extern "C" fn random_dataset(height: usize, width: usize) -> (Array2<f64>, Array2<f64>) {
  let theta = Array::random((width, 1), (Normal::new(0., 1.)));
  println!("Original theta when faking the dataset:");
  print_array(&theta);
  let features = Array::random((height, width), (Normal::new(0., 1.)));
  let exponent = -features.dot(&theta);
  let labels = scalar_divided(&scalar_add(&pointwise_exp(&exponent), 1.0), 1.0);
  return (features, labels);
}

#[no_mangle]
pub extern "C" fn test_dplr() {
  let (features, labels) = random_dataset(10000, 5);
  let theta = dp_logistic_regression(&features, &labels, 0.0, 0.01, 1.0, 0.01);
  println!("Theta got via logistic regression, should be times of the original one:");
  print_array(&theta);
}

fn main() {
  println!("Start...");
  test_dplr();
  println!("Finish...");
}
