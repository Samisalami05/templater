#![no_std]
#![no_main]

#![allow(unsafe_op_in_unsafe_fn, dead_code, unused)]

extern crate libc;

use core::{panic::PanicInfo};

#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn main() -> i32 {
    libc::printf(c"Welcome to \x1b[1;4;94mCrust\x1b[0m, the rust you always wanted.\n".as_ptr());
    0
}
