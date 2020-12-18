use serde::{Serialize, Deserialize};
use serde_json::from_reader;
use std::error::Error;
use std::io::{prelude::*, BufReader};
use std::fs::File;

use crate::jzazbz::JzAzBzColor;

const DEFAULT_SETTINGS_FILENAME : &str = "settings.json";

#[derive(Debug, Serialize, Deserialize)]
pub struct Settings {
	pub ui: UiSettings,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct UiSettings {
	pub colors: ColorSettings
}

#[derive(Debug, Serialize, Deserialize)]
pub struct ColorSettings {
	pub foreground: JzAzBzColor,
	pub background: JzAzBzColor,
}

impl Settings {
	pub fn load() -> Result<Self, Box<dyn Error>> {
		let file = File::open(DEFAULT_SETTINGS_FILENAME)?;
		let reader = BufReader::new(file);

		let s = serde_json::from_reader(reader)?;

		Ok(s)
	}
}

#[cfg(test)]
mod tests {
		use super::*;

		#[test]
		fn test_deserialize() {
				let settings = Settings::load().expect("Unable to load settings file");
				let foreground = settings.ui.colors.foreground;
				assert_eq!(foreground.j, 0.53125);
				assert_eq!(foreground.a, 0.983072937);
				assert_eq!(foreground.b, 0.458333343);
		}
}
