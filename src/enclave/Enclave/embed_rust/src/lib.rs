#![feature(prelude_import)]
#![no_std]

#[macro_use]
extern crate sgx_tstd as std;

#[allow(unused)]
#[prelude_import]
use std::prelude::v1::*;

#[no_mangle]
pub extern "C" fn hello_world() {
   
}
