use crate::jzazbz::JzAzBzColor;
use super::ColorError;
use serde::{Serialize, Serializer};

impl Serialize for JzAzBzColor {

fn serialize<S>(&self, _: S) -> std::result::Result<<S as Serializer>::Ok, <S as Serializer>::Error> where S: Serializer { todo!() }
}
