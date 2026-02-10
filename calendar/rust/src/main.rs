use std::env;
use std::process;

struct ZellersInputs {
    day: i32,
    month: i32,
    year: i32,
    century: i32,
}

fn zellers_congruence(inputs: &ZellersInputs) -> i32 {
    (inputs.day +
     (13 * (inputs.month + 1) / 5) +
     inputs.year +
     (inputs.year / 4) +
     (inputs.century / 4) - (2 * inputs.century)) % 7
}

const MONTHS: [&str; 12] = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
];

fn days_in_month(month: u32, year: u32) -> u32 {
    match month {
        1 | 3 | 5 | 7 | 8 | 10 | 12 => 31,
        4 | 6 | 9 | 11 => 30,
        2 => if is_leap_year(year) { 29 } else { 28 },
        _ => panic!("Invalid month"),
    }
}

fn is_leap_year(year: u32) -> bool {
    (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
}

fn print_calendar(day_of_week: u32, month: u32, year: u32) {
    println!("{:>41}", format!("{} {}", MONTHS[(month - 1) as usize], year));
    println!("{:<5} {:<5} {:<5} {:<5} {:<5} {:<5} {:<5}", "| Mon", "| Tue", "| Wed", "| Thu", "| Fri", "| Sat", "| Sun");

    let days_in_current_month = days_in_month(month, year);
    let shifted_day = (day_of_week + 5 ) % 7;
    let max_days = days_in_current_month + shifted_day;

    for i in 1..=max_days {
        if i > shifted_day {
            print!("{:<5} ", format!("| {:02}", i - shifted_day));
        } else {
            print!("      ");
        }
        if i % 7 == 0 {
            println!();
        }
    }

}

fn main() {
    let mut args = env::args();

    args.next(); // skip first argument which is always the program name

    let month: i32 = args
        .next()
        .expect("Missing month argument")
        .parse()
        .expect("Month must be an integer");

    let year: i32 = args
        .next()
        .expect("Missing year argument")
        .parse()
        .expect("Year must be an integer");

    if month < 1 || month > 12 {
        eprintln!("Error: month must be between 1 and 12.");
        process::exit(1);
    }

    let inputs = ZellersInputs {
        day: 1,
        month: if month <= 2 { month + 12 } else { month },
        year: if month <= 2 { (year - 1) % 100 } else { year % 100 },
        century: year / 100,
    };

    let day_of_week = zellers_congruence(&inputs);
    
    print_calendar(day_of_week as u32, month as u32, year as u32);

}
