use std::vec;
use project2::{
    dotproduct,
    dotproduct_parallel,
    dotproduct_simd,
    randomize_vec,
};

fn main() {
    // 示例用法
    let mut a = vec![1.0, 2.0, 3.0];
    randomize_vec(&mut a, 0.0..10.0);
    let b = vec![4.0, 5.0, 6.0];
    let result = dotproduct_simd(&a, &b, 3 as usize);
    let result_scalar = dotproduct(&a, &b, 3 as usize);
    let result_parallel = dotproduct_parallel(&a, &b, 3 as usize, 4);
    println!("Dot product: {}", result);
    println!("Dot product (scalar): {}", result_scalar);
    println!("Dot product (parallel): {}", result_parallel);
}
