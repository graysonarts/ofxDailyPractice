#[macro_use] extern crate pathsep;

use structopt::StructOpt;
use std::path::PathBuf;
use artcode::BatDate;
use chrono::{Local, NaiveDate};
use std::io::prelude::*;

mod colors;
mod vs;
mod cpp;
mod error;

use colors::random_color;

macro_rules! SKETCH_PATH {
    ($i:ident) => {
        format!(concat!("src", path_separator!(), "sketches", path_separator!(), "{}.h"), $i)
    };
}

#[derive(Debug, StructOpt)]
#[structopt(name = "daily", about = "Adds a new daily sketch to avoid all the boilerplate setup")]
struct Opt {
    #[structopt(short, long)]
    shame: bool,

    #[structopt(short, long, help="YYYYmmdd")]
    date: Option<String>,

    #[structopt(short, long)]
    label_color: Option<String>,

    #[structopt(short, long)]
    background_color: Option<String>,

    #[structopt(short, long)]
    from: Option<String>,

    #[structopt(name="PATH", parse(from_os_str))]
    project_dir: PathBuf,
}

fn calculate_project_name(date: &Option<String>) -> Result<String, error::RuntimeError> {
    let date : BatDate = match date {
        Some(d) => NaiveDate::parse_from_str(d, "%Y%m%d")
            .map_err(|_| error::RuntimeError::InvalidArgument(d.to_owned()))?
            .into(),
        None => Local::today().into()
    };
    let project_name = date.to_string().split_off(3).replace(".", "_");
    Ok(project_name)
}

fn write_template(data: &str, name: &str, root: &PathBuf, background: &str, label: &str) -> Result<(), std::io::Error> {
    let filename = root.join(SKETCH_PATH!(name));
    if filename.exists() {
        println!("File already exists, skipping writing the new file");
        return Ok(());
    } else {
        println!("Writing out: {}", filename.to_string_lossy());
    }

    let data = data.replace("{name}", name).replace("{background}", background).replace("{label}", label);

    let mut output_file = std::fs::File::create(filename)?;
    write!(output_file, "{}", data)
}

fn generate_new(project_name: &str, project_dir: &PathBuf, background: &str, label: &str) -> std::io::Result<()> {
    write_template(include_str!("normal.h.tmpl"), project_name, project_dir, background, label)
}

fn copy_from(name: &str, project_dir: &PathBuf, source: &str) -> std::io::Result<()> {
    let source_path = project_dir.join(SKETCH_PATH!(source));
    let dest_path = project_dir.join(SKETCH_PATH!(name));
    println!("{} -> {}", source_path.to_string_lossy(), dest_path.to_string_lossy());

    let data = std::fs::read_to_string(&source_path)?;
    let data = data.replace(source, name);
    let mut output_file = std::fs::File::create(dest_path)?;
    write!(output_file, "{}", data)
}

fn main() -> Result<(), std::io::Error> {
    let opt = Opt::from_args();

    let _ = opt.project_dir.exists() || Err(std::io::Error::new(std::io::ErrorKind::NotFound, "Could not find project dir"))?;
    let project_dir = opt.project_dir.canonicalize()?;
    let project_name = calculate_project_name(&opt.date)?;
    println!("Creating {}", project_name);

    let label = opt.label_color.unwrap_or_else(|| random_color().to_owned());
    let background = opt.background_color.unwrap_or_else(|| random_color().to_owned());

    let (macro_line, include) = match opt.shame {
        true => (r#"SHAME({name}, ofColor::{background}, ofColor::{label});"#,  "// {name} - SHAME!"),
        false => ( r#"SKETCH({name});"#, r#"#include "{name}.h""#)
    };

    if !opt.shame {
        match opt.from {
            Some(n) => copy_from(&project_name, &project_dir, &n)?,
            None => generate_new(&project_name, &project_dir, &background, &label)?

        }
    }
    cpp::add_sketch_entries(&project_name, &include, &macro_line, &opt.project_dir, &background, &label)?;
    if vs::has_project(&project_dir) && !opt.shame {
        vs::add_to_vcxproj(&project_dir, &project_name)?;
        vs::add_to_filters(&project_dir, &project_name)?;
    }




    Ok(())
}
