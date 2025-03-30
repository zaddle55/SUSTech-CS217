use criterion::{black_box, criterion_group, criterion_main, Criterion, BenchmarkId};
use project2::{dotproduct, dotproduct_parallel, dotproduct_simd, randomize_vec};
use std::time::Duration;

fn bench_size(c: &mut Criterion) {
    let mut group = c.benchmark_group("Vector Size Comparison");
    group.measurement_time(Duration::from_secs(10));
    
    for size in [10_000, 100_000, 1_000_000, 10_000_000, 100_000_000].iter() {
        // 为每个大小创建和初始化向量
        let mut v1 = vec![0.0; *size];
        let mut v2 = vec![0.0; *size];
        randomize_vec(&mut v1, 0.0..1.0);
        randomize_vec(&mut v2, 0.0..1.0);
        
        // 使用最优化的块大小
        let num_threads = rayon::current_num_threads();
        
        group.bench_with_input(BenchmarkId::new("Serial", size), size, |b, _| {
            b.iter(|| dotproduct(black_box(&v1), black_box(&v2), *size))
        });
        
        group.bench_with_input(BenchmarkId::new("Parallel", size), size, |b, _| {
            b.iter(|| dotproduct_parallel(black_box(&v1), black_box(&v2), *size, num_threads))
        });
        group.bench_with_input(BenchmarkId::new("SIMD", size), size, |b, _| {
            b.iter(|| dotproduct_simd(black_box(&v1), black_box(&v2), *size))
        });
    }
    group.finish();
}

fn bench_threads(c: &mut Criterion) {
    let mut group = c.benchmark_group("Thread Count Comparison");
    group.measurement_time(Duration::from_secs(10));
    
    let size = 100_000_000;
    let mut v1 = vec![0.0; size];
    let mut v2 = vec![0.0; size];
    randomize_vec(&mut v1, 0.0..1.0);
    randomize_vec(&mut v2, 0.0..1.0);
    
    for num_threads in [1, 2, 4, 8, 16].iter() {
        group.bench_with_input(BenchmarkId::new("Parallel", num_threads), num_threads, |b, _| {
            b.iter(|| dotproduct_parallel(black_box(&v1), black_box(&v2), size, *num_threads))
        });
    }
    group.finish();
}

criterion_group!(benches, bench_size, bench_threads);
criterion_main!(benches);