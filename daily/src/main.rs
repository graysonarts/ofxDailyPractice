use structopt::StructOpt;
use std::path::PathBuf;

#[derive(Debug, StructOpt)]
#[structopt(name = "daily", about = "Adds a new daily sketch to avoid all the boilerplate setup")]
struct Opt {
    #[structopt(short, long)]
    shame: bool,

    #[structopt(name="PATH", parse(from_os_str))]
    project_dir: PathBuf,
}

fn main() {
    let opt = Opt::from_args();
    println!("{:?}", opt);
}
