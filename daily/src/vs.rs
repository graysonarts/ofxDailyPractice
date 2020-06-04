use std::path::{ PathBuf, Path};
use std::io::prelude::*;

const VCXPROJ_TAG_LINE : &'static str = r#"<ClInclude Include="src\sketches\sketches.h" />"#;
const VCXPROJ_FILTER_LINE : &'static str = r#"<ClInclude Include="src\sketches\shame.h">
      <Filter>src\sketches</Filter>
    </ClInclude>"#;

pub fn has_project(path: &Path) -> bool {
	vcxproj(path).exists()
}

fn vcxproj(path: &Path) -> PathBuf {
	let stem = path.file_stem().unwrap().to_string_lossy();
	let filename = format!("{}.vcxproj", stem);

	path.join(filename)
}

fn filters(path: &Path) -> PathBuf {
	let stem = path.file_stem().unwrap().to_string_lossy();
	let filename = format!("{}.vcxproj.filters", stem);

	path.join(filename)
}

pub fn add_to_vcxproj(path: &Path, name: &str) -> std::io::Result<()> {
	let filename = vcxproj(path);
	let data = std::fs::read_to_string(&filename)?;

	let parts : Vec<_> = data.split(VCXPROJ_TAG_LINE).collect();
	let mut output_file = std::fs::File::create(filename)?;
	write!(output_file, "{}", parts[0])?;
	write!(output_file, r#"<ClInclude Include="src\sketches\{}.h" />"#, name)?;
	write!(output_file, "\n    {}", VCXPROJ_TAG_LINE)?;
	write!(output_file, "{}", parts[1])
}

pub fn add_to_filters(path: &Path, name: &str) -> std::io::Result<()> {
	let filename = filters(path);
	let data = std::fs::read_to_string(&filename)?;
	let parts : Vec<_> = data.split(VCXPROJ_FILTER_LINE).collect();
	let mut output_file = std::fs::File::create(filename)?;
	write!(output_file, "{}", parts[0])?;
	write!(output_file, r#"<ClInclude Include="src\sketches\{}.h">"#, name)?;
	write!(output_file, "\n      <Filter>src\\sketches</Filter>\n    </ClInclude>\n")?;
	write!(output_file, "    {}", VCXPROJ_FILTER_LINE)?;
	write!(output_file, "{}", parts[1])
}