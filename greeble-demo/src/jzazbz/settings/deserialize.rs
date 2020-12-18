use crate::jzazbz::JzAzBzColor;
use serde::{Deserialize, Deserializer, de, de::Visitor, de::SeqAccess};
use std::fmt::{self, Formatter};

impl<'de> Deserialize<'de> for JzAzBzColor {

fn deserialize<D>(d: D) -> Result<Self, D::Error> where D: Deserializer<'de> {
	const FIELDS: &'static [&'static str] = &["j", "a", "b"];
	d.deserialize_struct("JzAzBzColor", FIELDS, ColorVisitor)
 }
}

struct ColorVisitor;

impl<'de> Visitor<'de> for ColorVisitor {
	type Value = JzAzBzColor;

	fn expecting(&self, f: &mut Formatter) -> fmt::Result {
		f.write_str("JzAzBz Tuple")
	}

	fn visit_seq<V>(self, mut seq: V) -> Result<JzAzBzColor, V::Error> where V: SeqAccess<'de> {
		let jz = seq.next_element()?
			.ok_or_else(|| de::Error::invalid_length(0, &self))?;
			let az = seq.next_element()?
			.ok_or_else(|| de::Error::invalid_length(1, &self))?;
			let bz = seq.next_element()?
			.ok_or_else(|| de::Error::invalid_length(2, &self))?;

			Ok(JzAzBzColor::new(jz, az, bz))
	}
}
