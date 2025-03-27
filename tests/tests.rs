// tests/your_test.rs
use numvert::*;

#[test]
fn test_add() {
    let math_ops = assemble_math_ops(&matches);
    input = math_engine(input, math_ops.unwrap());
}