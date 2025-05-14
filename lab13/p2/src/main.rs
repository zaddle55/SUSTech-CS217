fn main() {
    // println!("Hello, world!");
    let s = give_ownership();
    println!("{}", s);
}

fn give_ownership() -> String {
    let s = String::from("Rust");
    s // s is returned and moved out of the function
}
