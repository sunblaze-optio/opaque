use std::f64::consts::E;
use std::error::Error;
use ndarray::{Array, Array1, Array2, Ix2};

pub extern "C" fn scalar_add(x: &Array2<f64>, s: f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()+s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}

pub extern "C" fn scalar_divide(x: &Array2<f64>, s:f64) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| i.clone()/s).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();

}


pub extern "C" fn pointwise_exp(x: &Array2<f64>) -> Array2<f64> {
  let v: Vec<f64> = x.iter().map(|i| E.powf(i.clone())).collect();
  let result = Array::from_shape_vec(x.shape(), v).unwrap();
  return result.into_dimensionality::<Ix2>().unwrap();
}
