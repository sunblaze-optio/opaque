#![feature(prelude_import)]
#![no_std]

#[macro_use]
extern crate sgx_tstd as std;

extern crate ndarray as ndarray;
extern crate ndarray_linalg_sgx as ndarray_linalg;

#[allow(unused)]
#[prelude_import]
use std::prelude::v1::*;

//mod clip;
//mod gradient;
//mod noise;

//use gradient::hubersvm_gradient;

#[no_mangle]
pub extern fn hello_world() {
   
}

#[no_mangle]
// deep copy or shallow copy?
pub extern fn passing(arr: &mut [f64], len: i32) {
  arr[1] = 0.0;
  /*for i in 0..len {
    let j = i as usize;
    arr[j] = 1.0;
  }*/
}
