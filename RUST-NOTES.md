# Rust notes

## Overview

- "Golden Rule": Read the compiler messages *very* carefully; the compiler is
  *extremely* clever, pedantic, but also helpful! Generally it tells you what
  the problem is *and* how to fix it!
- All variables should be in `snake_case` rather than golang's `camelCase` (the compiler will warn if not).
- All structs should be in `camelCase` (the compiler will warn if not).
- Whereas golang provides `interface`, rust uses `trait`'s (very extensively).
- All statements need to end in a semi-colon (like in C :)
- All variables are read-only by default.

  To make them modifiable, use the `mut` keyword:

  ```rust
  let num = 7;

  // ERROR: variable is read-only!
  num = 1;
  ```

  ```rust
  let mut num = 7;

  // Works
  num = 1;
  ```

## Files, packages and builds

- No files are imported automatically into `main.rs`.

  To import specify `mod $filename` *for every file*.

  ```rust
  // import "foo.rs"
  mod foo;

  // All modules are namespaced by their filename
  foo::func1();
  foo::func2();
  ```

- You can call the rust compiler (`rustc`) directly to build a single-file.
- Use the `cargo` command if you want to build a multi-file project.
- `cargo` expects your source files to live in `./src/`.

- `cargo` creates a `Cargo.toml` "manifest file" containing details of your
  project including any external dependencies (vendored code).

- Libraries/packages are called "crates" in rust terminology.

  Search for them using `cargo search`.

- When you run `cargo build` or `cargo run`, the crate will be downloaded
  automatically and compiled into your code.

- All crates use [semver](https://semver.org) versioning.

- To use a crate, add it's name and version to your `Cargo.toml` file.

  ```sh
  $ cargo search clap | head -1
  clap = "3.0.0-beta.1"              # A simple to use, efficient, and full-featured Command Line Argument Parser
  ```

  After adding those details to your `Cargo.toml`:

  ```sh
  $ sed -ne '/dependencies/,$ p' Cargo.toml
  [dependencies]
  clap = "3.0.0-beta.1"
  ```

- Once you have added the crate details to the manifest, make the crate
  available to your source file in `main.rs` using `use`:

  ```rust
  // Import 3 types from the clap crate
  use clap::{App, Arg, SubCommand};

  // Import all public symbols from the foo crate.
  use foo::*;

  // Import all public symbols from the bar crate.
  use bar;

  fn main() {
    let args = App::new("test app");

    // ...
  }
  ```

## Macros

- All macros are private by default.

  Use the `#[macro_export]` annotation to export one.

- The "unit type"

  `()` is a special type called the *unit type* that can be used anywhere rust
  expects a types to go.

  A common idiom is to use it for a functions where we only care about
  an error condition by specifying the function as returning
  `Result<()>`. This means effectively, "return nothing (the unit
  type) on success, but return a real error on failure.

## Enumerations (enums)

- Enums are fully namespaced - you need to specify
  `<enum-name>::<enum-value>` when using them:

  ```rust
  enum Thing {
    Foo,
    Bar,
    Baz,
  }

  // Enums are namespaced by their names too
  let thing = Thing::Foo;
  ```

- Enums don't only let you specify names for a list of values; they can also
  store data!

  ```rust
  enum Result {
    Ok(String),
    Err(String),
  }

  let good_result = Result::Ok("worked".to_string());
  let bad_result = Result::Err("failed".to_string());
  ```

  The `Result` example above is similar to the standard `Result` type, which is
  defined as:

  ```rust
  enum Result<T, E> {
    Ok(T),
    Err(E),
  }

  let good_result = Result::Ok(42);
  let bad_result = Result::Err("Doh");
  ```

  The `T` and `E` are generic types (they can be anything).

  Since `T` can be anything, For functions that only return an error, it's
  common to see the "unit type" used for the `T` type like this:

  ```rust
  fn foo() -> Result<(), String> {
    Ok(())
  }
  ```

  Hence, if an error is returned, there will be a string representation of it.
  But in the success case in this example, there is nothing meaninful to
  return, hence `Ok()` returns `()`.

## Optional values

- For optional values or values that could be `NULL` (C) or `nil` (go), use an
  `Option` type:

  ```rust

  fn foo(s: String, u: u64, b: bool) {
      println!("foo: s: {}, u: {}, b: {}", s, u, b);
  }

  type FP = fn(s: String, u: u64, b: bool);
 
  // Initially, our Option variable is "unset"
  let optional_function: Option<FP> = None;

  optional_function = Some(foo);

  // 
  let fp = optional_function.unwrap();

  fp("hello".to_string(), 123456789, true);
  ```

## Functions

- All functions are private by default.

  To make them public prefix with `pub`.

  ```rust
  fn private_func() { println!("I am private"); }

  pub fn public_func() { println!("I am public"); }
  ```

- All structures are private by default.

  To make them public prefix with `pub`.

- All structure *elements* are private by default.

  To make them public prefix with `pub`.

  ```rust
  pub struct Foo {
    pub name: String,
    pub age: u8,

    // This element is private
    password: String,
  }
  ```

- Functions should return either a `Result` or an `Option`.

- Returning a `Result` becomes very natural. In `golang`
  most functions return a value *and* an `error`. But in rust, you just return
  a `Result` which can return *either*! Compare:

  ```go
  // golang
  func get_string() (string, error) { ... }

  str, err := get_string()
  if err != nil {
      return err
  }
  ```

  ```rust
  // rust
  fn foo() -> Result<String, String> { ... }

  // This does the same as the golang example above. But it's much more
  // concise. The magic question mark ("?") means "if this function
  // returns and Err Result, return it back to the caller so entirely
  // replaces the golang "if err != nil { ... }" boiler plate code.
  let str = foo()?;
  ```

- If a function returns a `Result` or an `Option`, the compiler will expect
  you to "consume it" - either check the result, or explicitly ignore it by
  assigning to the `_` variable:

  ```rust
  // Explicitly ignore the return value of this function
  // by assigning the result to the variable `_` (like in go).
  let _ = func(1, 2, "foo"); 
  ```

- Functions can return multiple values using a tuple:

  ```rust
  fn return_multiple_values() -> (u64, i32, bool, String) {
      (123456789, -226526, true, String::from("moo"))
  }

  let results = return_multiple_values();
  println!("u64 value: {}", result.0);
  ```

## Matching

- Be very careful when using `match` with an enum!

  See if you can spot the mistake here:

  ```rust
  enum Thing {
      Foo,
      Bar,
  }

  let thing = Thing::Foo;

  // ERROR: incorrect code!
  match thing {
      Foo => println!("Got Foo"),
      Bar => println!("Got Bar"),
  };
  ```

  This looks reasonable _and it compiles_. However, it doesn't work as
  written: whatever value `thing` has, the code above will **always** display
  "Got Foo". Why?

  From https://doc.rust-lang.org/reference/patterns.html:

  > By default, identifier patterns **bind a variable** to a copy of or move
  > from the matched value...

  Still not clear? How about if we change that `match` code slightly:

  ```rust
  // ERROR: incorrect code!
  match thing {
      Foo => println!("Got Foo"),
      Bar => println!("Got Bar"),
      omg => println!("Got omg"),
      wtf => println!("Got wtf"),
      blah => println!("Got blah"),
  };
  ```

  This is _valid_ code: it compiles, but it's incorrect. Since we are not matching on
  _fully qualified enum values_, rust assumes you want to create a _variable_
  in each of the match arms so it creates three _variables_: `Foo`, `Bar` and
  `blah`! Worse still, since the code above is basically creating a set of
  variables that all handle "any" valid `Thing` value, the only arm which will
  **ever** match is the first one ("Got Foo")!

  What the programmer meant to write was this:

  ```rust
  match thing {
      Thing::Foo => println!("Got Thing::Foo"),
      Thing::Bar => println!("Got Thing::Bar"),
  };
  ```

  Note that the compiler will warn about this scenario. See
  https://doc.rust-lang.org/stable/nightly-rustc/rustc_lint_defs/builtin/static.BINDINGS_WITH_VARIANT_NAME.html
  for further details.

## Strings

- Rust has two types of string: `&str` (literal string) and
  `String` (vector of bytes):

  ```rust
  // Double-quoted string literal values are of type "&str"
  let str = "I am a str";

  // With explicit type specified
  let str2: &str = "I am also a str";

  let string = String::from("I am a String");

  // Explicit type specified
  let string2: String = String::from("I am also a String");
  ```

- Converting a `String` into a `&str`

  Either call `.as_str()` or just take a reference to the `String`:

  ```rust
  let string = String::from("foo");

  let str1 = &string;
  let str2 = string.as_str();
  ```

- Converting a `&str` into a `String`

  There are various ways. The following are essentially equivalent:

  ```rust
  // This is implicitly a "&str"
  let str1 = "hello";

  // Explicit specification of type
  let str2: &str = "world";

  let s1 = str1.to_string();
  let s2 = str1.to_owned();

  // ".into() is "magic" - it converts the thing it is called on into the
  // correct type. Since s3 is defined as type "String", ".into()" will
  // convert the str1 (&str) into a String!
  let s3: String = str1.into();
  ```

## Raw identifiers

Rust lets you create "raw string" literals using the special `r#` prefix and
`#` suffix on a string:

```rust
let s = r#"this string contains

embedded

   newlines and whitespace.
"#;
```

However, you can also use the magic `r#` prefix on identifiers! Check this
out:

```rust
fn r#fn() {
    let r#let = "let";

    let r#match = match r#let {
        "let" => true,
        _ => false,
    };

    let r#String: String = "a string".to_string();

    println!(
        r#"Hello from the r#fn function!
        r#let variable has value: {:?}
        r#match is: {:?}
        r#String is: {:?}
        "#,
        r#let, r#match, r#String
    );
}

fn call_weird_function() {
    r#fn();
}
```

Using a `r#` prefix, you can create variables and functions with the same
names as built-in identifiers or keywords like `fn`, `let`, `struct`, `match`, _etc_ (well, with the
addition of the odd `r#` prefix anyway ;)

A common use of this is for creating a variable called `type`. `type` on its
own is actually a rust keyword, but by prefixing with `r#`, you can create a
variable that is distinct from the keyword:

```rust
let r#type = "foo";

println!("type is: {:?}", r#type);
```

## Unit tests

- To mock with rust, use the `cfg` annotation. This allows you to create
  test-specific chunks of code:

  ```rust
  // Conditional assignment for tests using attributes.
  #[cfg(test)]
  const MULTIPLIER: u64 = 3;

  #[cfg(not(test))]
  const MULTIPLIER: u64 = 1;

  #[cfg(test)]
  fn handler() {
      println!("I am the test version of the function!");
  }

  #[cfg(not(test))]
  fn handler() {
      println!("I am the production version of the function!");
  }

  fn foo() {
      // The test version of the code
      #[cfg(test)]
      let x = 20;

      // The "production" version of the code
      #[cfg(not(test))]
      let x = -5;

      println!("INFO: x has value: {}", x);
  }

  // Non-test (production) version of the macro
  #[cfg(not(test))]
  macro_rules! welcome_all {
      ( $( $name:expr ),*) => {
          $( println!("INFO: PRODUCTION version: welcome_all: Hello {}", $name);)*
      };
  }

  // Test version of the macro
  #[cfg(test)]
  macro_rules! welcome_all {
      ( $( $name:expr ),*) => {
          $( println!("INFO: TEST version: welcome_all: Hello {}", $name);)*
      };
  }
  ```

  Now, when you run `cargo test` or `cargo tarpaulin`, the test versions will
  be used rather than the non-test versions.

## Gotchas

- The last statement in a function is the return type and must *NOT*
  end with a semi-colon:

  ```rust
  fn foo() -> Result<(), String> {
    // XXX: Error as there *is* no last statement due to the semi-colon!
    Ok(());
  }
  ```

  ```rust
  fn foo() -> Result<(), String> {
    // Correct
    Ok(())
  }
  ```

  This behaviour also affects blocks:

  ```rust
  let foo = {
    "hello";
  };

  println!("{:?}", foo);
  ```

  What do you think the code above displays? Hint: it does *not* display
  `hello`! What the user *actually* meant was probably:

  ```rust
  let foo = {
    "hello" // XXX: <-- Note that there is no semi-colon here
  };

  println!("{:?}", foo); // prints "hello" now ;)
  ```

  You can protect yourself against this sort of bug by always specifying the
  type of the variable:

  ```rust
  let foo: &str = {
    "hello";
  };
  ```

  Now that we've added the type of `foo` as `&str`, this will fail to compile.
  The correct version is:

  ```rust
  let foo: &str = {
    "hello" // <-- Note that there is no semi-colon here
  };
  ```

  Finally, this can affect you when running "unsafe" code using an `unsafe`
  block:

  ```rust
  // ERROR: incorrect code!
  // Oops! This creates an fd, then destroys it "returning" nothing (the unit type, "()")
  let file = unsafe { File::from_raw_fd(fd); };
  ```

  The corrected version (without the semi-colon in the `unsafe` block):
  ```rust
  let file: File = unsafe { File::from_raw_fd(fd) };
  ```

  Another "unsafe" gotcha using a `loop`. Spot the bug:

  ```rust
  // ERROR: incorrect code!
  loop {
      let mut reader = unsafe { File::from_raw_fd(read_fd) };

      // do something with the reader
  }
  ```

  The code above is *incorrect* since the `reader` goes out of scope each time
  through the loop. But that means the `File` object satisfying the `Read`
  trait gets garbage collected each time, and *that* means the underlying file
  descriptor (`read_fd`) is **closed** after the first iteration through the
  loop! The solution is simple - move the `reader` out of the loop:

  ```rust
  let mut reader = unsafe { File::from_raw_fd(read_fd) };

  loop {
      // do something with the reader
  }
  ```

- Lifetimes (advanced topic)

  In rust, all variables need to be "owned" by someone. This can make passing
  reference types (like `&str`) to functions difficult. Until you understand
  lifetimes, the best advice is to pass a `String` rather than a `&str` to a
  function (or make the function convert the `&str` into a `String`).

- If a function returns a `Result` and you don't check it, if an `Err` result
  is returned at runtime, the program will *panic*.

- Don't use `expect()` and `unwrap()` in real code.

  The following idioms will *also* cause panics if the functions fail:

  ```rust
  fn foo() -> Result<()> {
      let result1 = foo().expect("foo failed");

      let result2 = bar().unwrap();
  }
  ```

  Instead, pass the `Err` error back to the caller to deal with:

  ```rust
  fn foo() -> Result<()> {
      let result1 = foo();
      if result.is_err() {
          return result.err();
      }

      let result2 = bar()?;
      if result.is_err() {
          return result.err();
      }
  }
  ```

  This error handling code is quite "golang-like". But it isn't idiomatic
  rust. The standard convention (which looks much cleaner!) is to simply use
  the magic question mark which means "if the result is an error return it,
  else unpack/unwrap it". Like this:

  ```rust
  fn foo() -> Result<()> {
      let result1 = foo()?;

      let result2 = bar()?;
  }
  ```

- Be careful when implementing the `Display` and `Debug` traits to
  avoid a runtime *crash*:

  ```rust
  struct Foo {
      msg: String,
  }

  impl fmt::Display for Foo {
      fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
          write!(f, "{}", self.msg)
      }
  }

  impl fmt::Debug for Foo {
      fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
          // BUG: Attempting to write `self` below results in rust
          // calling *this method* to format the Foo passed to `write!()`.
          // But this method then calls itself again and again and again until
          // the program runs out of stack space!
          write!(f, "Debug: {:?}", self)
      }
  }

  fn main() {
      let foo = Foo{
          msg: "hello".into()
      };

      // Ok - this works as expected
      println!("foo (using Display trait): {}", foo);

      // BUG: this *crashes* the program! See comments above
      println!("foo (using Debug trait): {:?}", foo);
  }
  ```

## Manifest

- Manifest file

  - Your `Cargo.toml` `package.name` entry *MUST* match the directory
    your project lives in (aka don't modify it!)
  - Do *NOT* change the `package.edition` variable to the current year! :)
  - Do change the `package.version` variable as your project changes.

  ```toml
  [package]
  name = "test-foo"
  version = "0.1.0"
  authors = ["James O. D. Hunt <jamesodhunt@gmail.com>"]
  edition = "2018"

  [dependencies]
  clap = "3.0.0-beta.1"
  ```

## Tricks

- To find out the type of a variable, assign to a variable of type "unit". The
  compiler will generate an error showing the types of both variables:

  ```rust
  let x: () = var_with_unknown_type;
  ```
  Compiling will generate an error like this:

  ```
  error[E0308]: mismatched types
  --> test_var_of_unknown_type.rs:12:17
     |
  12 |     let x: () = var_with_unknown_type;
     |            --   ^^^^^^^^^^^^^^^^^^^^^ expected `()`, found struct `Foo`
     |            |
     |            expected due to this

  error: aborting due to previous error
  ```

  This shows that the `var_with_unknown_type` variable is of type `Foo`.

- Architecture specific code
  ```rust
  #[cfg(target_arch = "x86_64")]
  {
    // XXX: This block will only run on Intel 64-bit platforms!
  }
  ```
