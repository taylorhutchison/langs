use rand::random;
use memory_stats::memory_stats;

const NUM_COLUMNS: usize = 50;
const NUM_ROWS: usize = 20;
const GRID_SIZE: usize = NUM_COLUMNS * NUM_ROWS;

fn randomize_grid(grid: &mut [bool; GRID_SIZE]) {
    for i in 0..GRID_SIZE {
        grid[i] = random::<bool>();
    }
}

fn print_grid(grid: &[bool; GRID_SIZE]) {
    for row in 0..NUM_ROWS {
        for col in 0..NUM_COLUMNS {
            let index = (row * NUM_COLUMNS + col) as usize;
            if grid[index] {
                print!("X");
            } else {
                print!(" ");
            }
        }
        println!();
    }
}

fn number_of_neighbors(grid: &[bool; GRID_SIZE], row: usize, col: usize) -> u8 {
    let mut count = 0;
    for dr in [-1isize, 0, 1].iter() {
        for dc in [-1isize, 0, 1].iter() {
            if *dr == 0 && *dc == 0 {
                continue;
            }
            let neighbor_row = row as isize + dr;
            let neighbor_col = col as isize + dc;
            if neighbor_row >= 0
                && neighbor_row < NUM_ROWS as isize
                && neighbor_col >= 0
                && neighbor_col < NUM_COLUMNS as isize
            {
                let index = (neighbor_row as usize * NUM_COLUMNS + neighbor_col as usize) as usize;
                if grid[index] {
                    count += 1;
                }
            }
        }
    }
    count
}

fn update_grid(current: &[bool; GRID_SIZE], next: &mut [bool; GRID_SIZE]) {
    for row in 0..NUM_ROWS {
        for col in 0..NUM_COLUMNS {
            let index = (row * NUM_COLUMNS + col) as usize;
            let neighbors = number_of_neighbors(current, row, col);
            next[index] = match (current[index], neighbors) {
                (true, 2) | (true, 3) => true,
                (false, 3) => true,
                _ => false,
            };
        }
    }
}

fn clear_console() {
    print!("\x1B[2J\x1B[1;1H");
}

fn print_memory_usage() {
    if let Some(usage) = memory_stats() {
        println!("Working set: {} KB", usage.physical_mem / 1024);
    }
}

fn main() {
    let mut grid1 = [false; GRID_SIZE];
    let mut grid2 = [false; GRID_SIZE];
    randomize_grid(&mut grid1);

    let mut current = &mut grid1;
    let mut next = &mut grid2;

    loop {
        clear_console();
        print_grid(current);
        update_grid(current, next);
        print_memory_usage();
        std::mem::swap(&mut current, &mut next);
        std::thread::sleep(std::time::Duration::from_millis(200));
    }
}
