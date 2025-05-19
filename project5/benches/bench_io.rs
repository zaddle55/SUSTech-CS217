use criterion::{criterion_group, criterion_main, Criterion, BenchmarkId};
use image::{ImageReader, Luma, GrayImage};

use std::{string, time::Duration};

const IMAGE_DIR: &str = "img/";

fn decode_1cNxN(c: &mut Criterion) {
    let mut group = c.benchmark_group("Decode_1cNxN");
    group.measurement_time(Duration::from_secs(10));

    for size in [128u32, 256, 512, 1024, 2048, 4096, 8192].iter() {
        let fname = format!("{}1c{}x{}.bmp", IMAGE_DIR, size, size);
        group.bench_with_input(BenchmarkId::new("Decode 1cNxN BMP", size), 
        size,
        |b, _| {
            b.iter(|| {
                let _ = ImageReader::open(&fname)
                    .expect("Failed to open image")
                    .decode()
                    .expect("Failed to decode image");
            })
        });
    }
    group.finish();
}

fn encode_1cNxN(c: &mut Criterion) {
    let mut group = c.benchmark_group("Encode_1cNxN");
    group.measurement_time(Duration::from_secs(10));

    for size in [128u32, 256, 512, 1024, 2048, 4096, 8192].iter() {
        let fname = format!("{}1c{}x{}.bmp", IMAGE_DIR, size, size);
        let mut img = GrayImage::new(size.to_owned(), size.to_owned());
        for y in 0..*size {
            for x in 0..*size {
                img.put_pixel(x, y, Luma([255]));
            }
        }
        group.bench_with_input(BenchmarkId::new("Encode 1cNxN BMP", size),
        size,
        |b, _| {
            b.iter(|| {
                img.save(&fname).expect("Failed to save image");
            })
        });
    }
    group.finish();
}

criterion_group!(benches, decode_1cNxN, encode_1cNxN);
criterion_main!(benches);