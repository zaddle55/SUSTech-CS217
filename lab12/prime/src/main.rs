use std::io;
fn main() {
    // read a number from the user

    // let a = 16i16;
    // let b = 7;
    // let c = a + b;
    let mut input = String::new();
    println!("Enter a number:");
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");
    let num: u32 = input.trim().parse().expect("Please enter a valid number");
    // check if the number is prime(divide and conquer)
    let mut is_prime = true;
    if num < 2 {
        is_prime = false;
    } else {
        for i in 2..=((num as f64).sqrt() as u32) {
            if num % i == 0 {
                is_prime = false;
                break;
            }
        }
    }

    println!("The number {} is {}", num, if is_prime { "prime" } else { "not prime" });
}
