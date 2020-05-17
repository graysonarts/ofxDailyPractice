use structopt::StructOpt;
use std::path::PathBuf;
use artcode::BatDate;
use chrono::Local;
use std::io::prelude::*;

mod colors;

use colors::random_color;

#[derive(Debug, StructOpt)]
#[structopt(name = "daily", about = "Adds a new daily sketch to avoid all the boilerplate setup")]
struct Opt {
    #[structopt(short, long)]
    shame: bool,

    #[structopt(short, long)]
    label_color: Option<String>,

    #[structopt(short, long)]
    background_color: Option<String>,

    #[structopt(name="PATH", parse(from_os_str))]
    project_dir: PathBuf,
}

fn calculate_project_name() -> Result<String, std::io::Error> {
    let date : BatDate = Local::today().into();
    let project_name = date.to_string().split_off(3).replace(".", "_");
    Ok(project_name)
}

fn add_sketch_entries(name: &str, include: &str, macro_line: &str, root: &PathBuf, background: &str, label: &str) -> Result<(), std::io::Error> {
    let filename = root.join("src/sketches/sketches.h");
    let data = std::fs::read_to_string(&filename)?;
    let split1 : Vec<_> = data.split("/// INCLUDE").collect();
    let split2 : Vec<_> = split1[1].split("/// SKETCH LIST").collect();

    let mut output_file = std::fs::File::create(filename)?;
    write!(output_file, "{}", split1[0])?;
    write!(output_file, "{}\n", include.replace("{name}", name).replace("{background}", background).replace("{label}", label))?;
    write!(output_file, "/// INCLUDE\n")?;
    write!(output_file, "{}", split2[0])?;
    write!(output_file, "/// SKETCH LIST\n")?;
    write!(output_file, "\t{}", macro_line.replace("{name}", name).replace("{background}", background).replace("{label}", label))?;
    write!(output_file, "{}", split2[1])?;

    Ok(())
}

fn write_template(data: &str, name: &str, root: &PathBuf, background: &str, label: &str) -> Result<(), std::io::Error> {
    let filename = root.join(format!("src/sketches/{}.h", name));
    let data = data.replace("{name}", name).replace("{background}", background).replace("{label}", label);

    let mut output_file = std::fs::File::create(filename)?;
    write!(output_file, "{}", data)
}

fn main() -> Result<(), std::io::Error> {
    let opt = Opt::from_args();

    let _ = opt.project_dir.exists() || Err(std::io::Error::new(std::io::ErrorKind::NotFound, "Could not find project dir"))?;
    let project_name = calculate_project_name()?;
    println!("Creating {}", project_name);

    let (template, macro_line, include) = match opt.shame {
        true => (None, r#"SHAME({name}, ofColor::{background}, ofColor::{label});"#, "// {name} - SHAME!"),
        false => (Some(include_str!("normal.h.tmpl")), r#"SKETCH({name});"#, r#"#include "{name}.h""#),
    };

    let label = opt.label_color.unwrap_or_else(|| random_color().to_owned());
    let background = opt.background_color.unwrap_or_else(|| random_color().to_owned());

    add_sketch_entries(&project_name, &include, &macro_line, &opt.project_dir, &background, &label)?;

    match template {
        Some(data) => write_template(data, &project_name, &opt.project_dir, &background, &label)?,
        _ => ()
    };


    Ok(())
}
