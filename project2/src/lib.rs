
use std::ops::{Add, Mul, Range};

// use criterion::{black_box, criterion_group, criterion_main, BenchmarkId, Criterion};
use rayon::prelude::*;

use rand::{distributions::uniform::{SampleRange, SampleUniform}, Rng};

pub fn randomize_vec<T>(vec: &mut Vec<T>, range: Range<T>) 
where 
    T: Copy + SampleUniform,
    rand::distributions::Standard: rand::distributions::Distribution<T>,
    Range<T>: SampleRange<T>,
{
    let mut rng = rand::thread_rng();
    for item in vec.iter_mut() {
        *item = rng.gen_range(range.clone());
    }
}

pub fn dotproduct<T>(a: &[T], b: &[T], c: usize) -> T 
where 
    T: std::ops::Mul<Output = T> + std::ops::AddAssign + Copy + Default 
{
    let mut result = T::default();
    for i in 0..c {
        result += a[i] * b[i];
    }
    result
}

pub fn dotproduct_parallel<T>(a: &[T], b: &[T], size: usize, num_threads: usize) -> T
where 
    T: Send + Sync + Copy + Add<Output = T> + std::iter::Sum + Mul<Output = T> + Default,
{
    let chunk_size = (size + num_threads - 1) / num_threads;
    a.par_chunks(chunk_size)
        .zip(b.par_chunks(chunk_size))
        .map(|(chunk1, chunk2)| {
            chunk1.iter()
                .zip(chunk2.iter())
                .map(|(&a, &b)| a * b)
                .sum()
        })
        .sum::<T>()
}

#[cfg(target_arch = "x86_64")]
pub fn dotproduct_simd(a: &[f32], b: &[f32], c: usize) -> f32 {
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

#[cfg(target_arch = "aarch64")]
pub fn dotproduct_simd(a: &[f32], b: &[f32], c: usize) -> f32 {
    use std::arch::aarch64::*;
    let mut result = unsafe {vdupq_n_f32(0.0)};
    let mut i = 0;
    unsafe {while i + 2 <= c {
        let a_vec = vld1_s32(&a[i]);
        let b_vec = vld1_s32(&b[i]);
        result = vmull_s32(a_vec, b_vec);
        i += 4;
    }}
    let mut sum = [0.0; 4];
    unsafe {sum = vaddvq_f32(result)};
    sum.iter().sum::<f32>() + dotproduct(&a[i..], &b[i..], c - i)
}