use std::mem::{size_of_val, align_of_val};

struct RustStyle {
    a: u8,  // 1 byte
    b: u32, // 4 bytes
    c: u16, // 2 bytes
    d: f64, // 8 bytes
}

#[repr(C)]
struct CStyle {
    a: u8,  // 1 byte
    b: u32, // 4 bytes
    c: u16, // 2 bytes
    d: f64, // 8 bytes
}

fn print_struct<T>(instance: &T, name: &str) {
    println!("\n=== {} Structure ===", name);
    println!("Size: {} Bytes", size_of_val(instance));
    println!("Alignment of instance: {} Bytes", align_of_val(instance));
}

fn main() {
    let rust_instance = RustStyle { a: 1, b: 2, c: 3, d: 4.0 };
    let c_instance = CStyle { a: 1, b: 2, c: 3, d: 4.0 };

    print_struct(&rust_instance, "Rust optimized layout");
    print_struct(&c_instance, "C layout");
}