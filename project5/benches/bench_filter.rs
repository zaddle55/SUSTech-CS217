use imageproc::{definitions::Image, filter::{self, Kernel}, map::ChannelMap};
use image::{ImageBuffer, ImageReader, Pixel, Rgb, RgbImage};
use criterion::{criterion_group, criterion_main, Criterion, BenchmarkId};

const IMAGE_DIR: &str = "img/";

fn filter_NxN(c: &mut Criterion) {
    let mut group = c.benchmark_group("Filter_NxN");
    group.measurement_time(std::time::Duration::from_secs(15));
    // use us as time unit
    group.sample_size(10);
    group.warm_up_time(std::time::Duration::from_secs(5));
    group.throughput(criterion::Throughput::Elements(1));

    for (w, h) in [
    (300, 200),
    (600, 400),
    (900, 600),
    (1200, 800),
    (1800, 1200),
    (2400, 1600),
    (3000, 2000),
    (3600, 2400),
    (6000, 4000)].iter() {
        let fname = format!("{}sample_{}x{}.jpg", IMAGE_DIR, w, h);
        let img = image::open(&fname).expect("Failed to open image").to_luma8();
        // let kernel = &[1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
        //       1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
        //       1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0];
        group.bench_with_input(BenchmarkId::new("Filter NxN RGB Image", w), w, |b, _| {
            b.iter(|| {
                let _ = filter::box_filter(&img, 1, 1);
                // let _: Image<ChannelMap<Rgb<u8>, u8>> = filter::filter3x3(&img, kernel);
            });
        });
    }
    group.finish();
}

fn filter_KxK(c: &mut Criterion) {
    let mut group = c.benchmark_group("Filter_KxK");
    group.measurement_time(std::time::Duration::from_secs(15));
    // use us as time unit
    group.sample_size(10);
    group.warm_up_time(std::time::Duration::from_secs(5));
    group.throughput(criterion::Throughput::Elements(1));

    for k in [3, 5, 7, 9, 11].iter() {
        let fname = format!("{}sample_600x400.jpg", IMAGE_DIR);
        let img = image::open(&fname).expect("Failed to open image").to_luma8();
        let kernel = vec![1.0 / (*k * *k) as f32; (*k * *k) as usize];
        group.bench_with_input(BenchmarkId::new("Filter KxK RGB Image", k), k, |b, _| {
            b.iter(|| {
                let _ = filter::box_filter(&img, k >> 1, k >> 1);
                // let _: Image<ChannelMap<Rgb<u8>, u8>> = filter::filter3x3(&img, &kernel);
            });
        });

    }
    group.finish();
}

criterion_group!(benches, filter_NxN, filter_KxK);
criterion_main!(benches);