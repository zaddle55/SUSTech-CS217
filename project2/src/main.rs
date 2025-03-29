use rand::{distributions::uniform::{SampleRange, SampleUniform}, Rng};

#[allow(unused)]
fn vec_random<T, R>(a: &mut [T], range: R, size: usize)
where 
    T: Copy + Default + std::ops::AddAssign + std::ops::Mul<Output = T> + SampleUniform,
    R: SampleRange<T> + Copy,
{
    let mut rng = rand::thread_rng();
    for i in 0..size {
        a[i] = rng.gen_range(range.clone());
    }
}

#[inline(never)]
fn dotproduct<T>(a: &[T], b: &[T], c: usize) -> T 
where 
    T: std::ops::Mul<Output = T> + std::ops::AddAssign + Copy + Default 
{
    let mut result = T::default();
    for i in 0..c {
        result += a[i] * b[i];
    }
    result
}

#[cfg(target_arch = "x86_64")]
fn dotproduct_simd(a: &[f32], b: &[f32], c: usize) -> f32 {
    use std::arch::x86_64::*;
    let mut result = unsafe {_mm_setzero_ps()};
    let mut i = 0;
    unsafe {while i + 4 <= c {
        let a_vec = _mm_loadu_ps(&a[i]);
        let b_vec = _mm_loadu_ps(&b[i]);
        result = _mm_add_ps(result, _mm_mul_ps(a_vec, b_vec));
        i += 4;
    }}
    let mut sum = [0.0; 4];
    unsafe {_mm_storeu_ps(&mut sum[0], result)};
    sum.iter().sum::<f32>() + dotproduct(&a[i..], &b[i..], c - i)
}

fn main() {
    // 示例用法
    let a = vec![1.0, 2.0, 3.0];
    let b = vec![4.0, 5.0, 6.0];
    let result = dotproduct_simd(&a, &b, 3 as usize);
    println!("Dot product: {}", result);
}
