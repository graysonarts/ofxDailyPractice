pub mod deserialize;
pub mod serialize;
use crate::jzazbz::JzAzBzColor;

use std::error::Error;
use serde::de::Error as DeError;

#[derive(Debug)]
pub struct ColorError {

}

impl std::fmt::Display for ColorError {

fn fmt(&self, _: &mut std::fmt::Formatter<'_>) -> std::result::Result<(), std::fmt::Error> { todo!() }
}

impl Error for ColorError {

}

impl DeError for ColorError { fn custom<T>(_: T) -> Self where T: std::fmt::Display { todo!() }
}
