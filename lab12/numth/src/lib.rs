pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

fn gcd(a: u8, b: u8) -> u8 {
    let mut a = a;
    let mut b = b;

    while b != 0 {
        let t = b;
        b = a % b;
        a = t;
    }
    a
}

pub fn lcm(a: u8, b: u8) -> u8 {
    a * b / gcd(a, b)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lcm() {
        assert_eq!(lcm(1, 4), 4);
        assert_eq!(lcm(8, 9), 72);
        assert_eq!(lcm(8, 16), 16);
        // assert_eq!(lcm(1024, 2), 1024);
        // assert_eq!(lcm(256, 2), 256);
        // assert_eq!(lcm(256, 0), 0);
        assert_eq!(lcm(64, 2), 64);
        assert_eq!(lcm(128, 0), 0);
    }
}
