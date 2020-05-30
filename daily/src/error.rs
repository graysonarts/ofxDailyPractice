use std::error::Error;
use std::fmt::Display;

#[derive(Debug)]
pub enum RuntimeError {
	IOError(std::io::Error),
	InvalidArgument(String)
}

impl Display for RuntimeError {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::result::Result<(), std::fmt::Error> {
		 match self {
			Self::IOError(e) => write!(f, "{}", e),
			Self::InvalidArgument(s) => write!(f, "Invalid Argument: {}", s),
		}
	 }
}

impl Error for RuntimeError { }

impl From<std::io::Error> for RuntimeError {
	fn from(e: std::io::Error) -> Self {
		Self::IOError(e)
	}
}

impl From<RuntimeError> for std::io::Error {
	fn from(e: RuntimeError) -> Self {
		match e {
			RuntimeError::IOError(e) => e,
			RuntimeError::InvalidArgument(_) => std::io::Error::new(std::io::ErrorKind::InvalidInput, e)
		}
	}
}