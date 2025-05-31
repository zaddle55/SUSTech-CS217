use image::{ImageReader};

const IMAGE_DIR: &str = "img/";

fn main() {
    let img = ImageReader::open(format!("{}1c8192x8192.bmp", IMAGE_DIR))
        .expect("Failed to open image")
        .decode()
        .expect("Failed to decode image")
        .to_rgb8();

    // write the image to a new file
    img.save(format!("{}1c8192x8192_copy.bmp", IMAGE_DIR))
        .expect("Failed to save image");

    let img = ImageReader::open(format!("{}1c4096x4096.bmp", IMAGE_DIR))
        .expect("Failed to open image")
        .decode()
        .expect("Failed to decode image")
        .to_luma8();
    // write the image to a new file
    img.save(format!("{}1c4096x4096_copy.bmp", IMAGE_DIR))
        .expect("Failed to save image");

    let img = ImageReader::open(format!("{}3c4096x4096.bmp", IMAGE_DIR))
        .expect("Failed to open image")
        .decode()
        .expect("Failed to decode image")
        .to_rgb8();
    // write the image to a new file
    img.save(format!("{}3c4096x4096_copy.bmp", IMAGE_DIR))
        .expect("Failed to save image");
    let img = ImageReader::open(format!("{}4c4096x4096.bmp", IMAGE_DIR))
        .expect("Failed to open image")
        .decode()
        .expect("Failed to decode image")
        .to_rgba8();
    // write the image to a new file
    img.save(format!("{}4c4096x4096_copy.bmp", IMAGE_DIR))
        .expect("Failed to save image");
}
