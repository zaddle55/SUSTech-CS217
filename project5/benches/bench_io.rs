use criterion::{criterion_group, criterion_main, Criterion, BenchmarkId};
use image::{DynamicImage, GenericImage, GrayImage, ImageBuffer, ImageReader, Luma, Pixel, Rgba};

use std::{time::Duration};

const IMAGE_DIR: &str = "img/";

fn decode_1cNxN(c: &mut Criterion) {
    let mut group = c.benchmark_group("Decode_1cNxN");
    group.measurement_time(Duration::from_secs(10));

    for size in [128u32, 256, 512, 1024, 2048, 4096, 8192].iter() {
        let fname = format!("{}1c{}x{}.bmp", IMAGE_DIR, size, size);
        group.bench_with_input(BenchmarkId::new("Decode 1cNxN BMP", size), 
        size,
        |b, _| {
            #[cfg(mem)] // iter once
            {
                let _ = ImageReader::open(&fname)
                        .expect("Failed to open image")
                        .decode()
                        .expect("Failed to decode image");
            }
            #[cfg(not(mem))]
            {
                b.iter(|| {
                    let _ = ImageReader::open(&fname)
                        .expect("Failed to open image")
                        .decode()
                        .expect("Failed to decode image");
                })
            }
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

fn encode_Nc4096x4096(c: &mut Criterion) {
    let mut group = c.benchmark_group("Encode_Nc4096x4096");
    group.measurement_time(Duration::from_secs(10));

    for channel in [1, 3, 4].iter() {
        let fname = format!("{}{}c4096x4096.bmp", IMAGE_DIR, channel);
        let mut img: DynamicImage;
        match channel {
            1 => img = DynamicImage::new_luma8(4096, 4096),
            3 => img = DynamicImage::new_rgb8(4096, 4096),
            4 => img = DynamicImage::new_rgba8(4096, 4096),
            _ => unreachable!(),
        }
        for y in 0..4096 {
            for x in 0..4096 {
                img.put_pixel(x, y, Rgba([255, 0, 0, 255]));
            }
        }
        group.bench_with_input(BenchmarkId::new("Encode Nc4096x4096 BMP", channel),
        channel,
        |b, _| {
            b.iter(|| {
                img.save(&fname).expect("Failed to save image");
            })
        });
    }
    group.finish();
}

fn decode_Nc4096x4096(c: &mut Criterion) {
    let mut group = c.benchmark_group("Decode_Nc4096x4096");
    group.measurement_time(Duration::from_secs(10));

    for channel in [1, 3, 4].iter() {
        let fname = format!("{}{}c4096x4096.bmp", IMAGE_DIR, channel);
        group.bench_with_input(BenchmarkId::new("Decode Nc4096x4096 BMP", channel),
        channel,
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

fn encode_1cNxM(c: &mut Criterion) {
    let mut group = c.benchmark_group("Encode_1cNxM");
    group.measurement_time(Duration::from_secs(10));

    for (w, h) in [(240, 4860), (360, 3240), (480, 2430), (720, 1620), (810, 1440), (1080, 1080), (1440, 810), (1620, 720), (2430, 480), (3240, 360), (4860, 240)].iter() {
        let fname = format!("{}1c{}x{}.bmp", IMAGE_DIR, w, h);
        let mut img = GrayImage::new(*w, *h);
        for y in 0..*h {
            for x in 0..*w {
                img.put_pixel(x, y, Luma([255]));
            }
        }
        group.bench_with_input(BenchmarkId::new("Encode 1cNxM BMP", w),
        w,
        |b, _| {
            b.iter(|| {
                img.save(&fname).expect("Failed to save image");
            })
        });
    }
    group.finish();
}

fn decode_1cNxM(c: &mut Criterion) {
    let mut group = c.benchmark_group("Decode_1cNxM");
    group.measurement_time(Duration::from_secs(10));

    for (w, h) in [(240, 4860), (360, 3240), (480, 2430), (720, 1620), (810, 1440), (1080, 1080), (1440, 810), (1620, 720), (2430, 480), (3240, 360), (4860, 240)].iter() {
        let fname = format!("{}1c{}x{}.bmp", IMAGE_DIR, w, h);
        group.bench_with_input(BenchmarkId::new("Decode 1cNxM BMP", w),
        w,
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


criterion_group!(benches,
    decode_1cNxN, encode_1cNxN,
    encode_Nc4096x4096, decode_Nc4096x4096,
    encode_1cNxM, decode_1cNxM);
criterion_main!(benches);