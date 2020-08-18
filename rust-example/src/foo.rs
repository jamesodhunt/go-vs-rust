//--------------------------------------------------------------------
// Comments
//--------------------------------------------------------------------

#[derive(Debug)]
pub struct Foo {
    pub name: String,
    age: u8,
}

impl Foo {
    pub fn new(name: &str, age_str: &str) -> Result<Foo, String> {
        if name == "" {
            return Err("need non blank name".to_string());
        }

        let age = match age_str.parse::<u8>() {
            Ok(age) => age,
            Err(e) => return Err(e.to_string()),
        };

        if age > 120 {
            return Err("invalid age".to_string());
        }

        let foo = Foo {
            name: name.to_string(),
            age: age as u8,
        };

        Ok(foo)
    }
}
