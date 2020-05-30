use std::io::prelude::*;
use std::path::PathBuf;

pub fn add_sketch_entries(name: &str, include: &str, macro_line: &str, root: &PathBuf, background: &str, label: &str) -> Result<(), std::io::Error> {
	let filename = root.join(join_path!("src", "sketches", "sketches.h"));
	let data = std::fs::read_to_string(&filename)?;
	let split1 : Vec<_> = data.split("/// INCLUDE").collect();
	let split2 : Vec<_> = split1[1].split("/// SKETCH LIST").collect();

	let mut output_file = std::fs::File::create(filename)?;
	write!(output_file, "{}", split1[0])?;
	write!(output_file, "{}\n", include.replace("{name}", name).replace("{background}", background).replace("{label}", label))?;
	write!(output_file, "/// INCLUDE")?;
	write!(output_file, "{}", split2[0])?;
	write!(output_file, "/// SKETCH LIST\n")?;
	write!(output_file, "\t{}", macro_line.replace("{name}", name).replace("{background}", background).replace("{label}", label))?;
	write!(output_file, "{}", split2[1])?;

	Ok(())
}