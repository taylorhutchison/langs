use std::fs::File;
use std::io::{self, Read, Seek, SeekFrom};
use std::collections::HashMap;

fn main() -> io::Result<()> {
    let mut file = File::open("../shared_data/image1.bmp")?;
    let mut header = [0u8; 54];
    file.read_exact(&mut header)?;

    let offset = i32::from_le_bytes(header[10..14].try_into().unwrap()) as u64;
    let width = i32::from_le_bytes(header[18..22].try_into().unwrap());
    let height = i32::from_le_bytes(header[22..26].try_into().unwrap());
    let bits_per_pixel = u16::from_le_bytes(header[28..30].try_into().unwrap());

    println!("Width: {}", width);
    println!("Height: {}", height);
    println!("Bits per pixel: {}", bits_per_pixel);

    let padded_width = (width * 3 + 3) & (!3);

    file.seek(SeekFrom::Start(offset))?;

    let mut row_buffer = vec![0u8; padded_width as usize];
    let mut rgb_counts: HashMap<(u8, u8, u8), u32> = HashMap::new();

    for _ in 0..height {
        file.read_exact(&mut row_buffer)?;
        for x in 0..width {
            let i = (x * 3) as usize;
            let blue = row_buffer[i];
            let green = row_buffer[i + 1];
            let red = row_buffer[i + 2];
            let rgb = (red, green, blue);
            *rgb_counts.entry(rgb).or_insert(0) += 1;
        }
    }

    let mut rgb_vec: Vec<((u8, u8, u8), u32)> = rgb_counts.into_iter().collect();
    rgb_vec.sort_by(|a, b| b.1.cmp(&a.1));

    for (_, (rgb, count)) in rgb_vec.iter().take(10).enumerate() {
        println!(
            "R: {} G: {} B: {} - Count {}",
            rgb.0, rgb.1, rgb.2, count
        );
    }

    Ok(())
}
