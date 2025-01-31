use std::env;
use std::process::exit;

mod foo;

fn test_foo(name: &str, age: &str) -> Result<(), String> {
    // Note: This is the "long-hand" approach, which is easier to understand.
    //
    // The experienced programmer can collapse all this code into just "?" ! :)
    let foo = match foo::Foo::new(name, age) {
        Ok(foo) => foo,
        Err(e) => return Err(e),
    };

    println!("Foo: {:?}", foo);

    Ok(())
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        eprintln!("Usage: {} <name> <age>", args[0]);
        exit(1);
    }

    let name = &args[1];
    let age = &args[2];

    let result = test_foo(name, age);

    if result.is_err() {
        eprintln!("ERROR: failed: {:?}\n", result.err());
        exit(1);
    }
}
