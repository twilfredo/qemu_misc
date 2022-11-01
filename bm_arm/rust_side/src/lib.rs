#![no_std]

#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn rs_hook_c(val: i32) -> i32;
        fn rs_build_token_array(buffer: &'static [u8], work_index: &mut i32) -> i32;
        fn rs_ping_cpp() -> bool;
    }

    unsafe extern "C++" {
        include!("rust_side/include/cpp_funcs.h");

        // C++ Functions
        fn cpp_hook_a() -> i32;
    }
}

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

pub fn rs_hook_c(val: i32) -> i32 {
    
    val.saturating_mul(10)
}

pub fn rs_ping_cpp() -> bool {
    let magic_num  = 1313;
    if ffi::cpp_hook_a() == magic_num {
        return true;
    }
    false
}

fn rs_build_token_array(buffer: &'static [u8], work_index: &mut i32) -> i32 {
    let mut count = 0;
    for i in buffer {
        count += *i as i32;
    }
    *work_index = count;
    -10
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn trivial_assertion() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }

    #[test]
    fn test_hookc() {
        assert_eq!(rs_hook_c(10), 100);
    }
}

#[cfg(not(test))]
#[cfg(not(feature = "simulation"))]
use core::panic::PanicInfo;

#[cfg(not(test))]
#[cfg(not(feature = "simulation"))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
