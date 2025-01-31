//--------------------------------------------------------------------
// Comments
//--------------------------------------------------------------------

#[derive(Debug)]
pub struct Foo {
    pub name: String,
    age: u8,
}

// A bit hopeful, maybe ;)
const AGE_MAX: usize = 120;

impl Foo {
    pub fn new(name: &str, age_str: &str) -> Result<Foo, String> {
        if name.is_empty() {
            return Err("need non blank name".to_string());
        }

        if age_str.is_empty() {
            return Err("need non blank age".to_string());
        }

        let age = match age_str.parse::<u8>() {
            Ok(age) => age,
            Err(e) => return Err(e.to_string()),
        };

        // Check hard limit
        if age == u8::MAX {
            return Err("invalid age".to_string());
        }

        // Check soft limit
        // Note the "cast"!
        if age > AGE_MAX as u8 {
            return Err("nobody's that old!".to_string());
        }

        // Allocate a _new_ name
        let name = name.to_string();

        let foo = Foo { name, age };

        Ok(foo)
    }
}
