use sdl2::pixels::Color;
use std::f64::consts::PI;

mod settings;

#[derive(Debug)]
pub struct JzAzBzColor {
	pub j: f64,
	pub a: f64,
	pub b: f64
}

fn scale_u8(f: f64) -> u8 {
	let scaled = (f * 255.0).clamp(0., 255.).round();

	unsafe { scaled.to_int_unchecked() }
}

impl JzAzBzColor {
	pub fn new(j: f64, a: f64, b: f64) -> Self {
		// TODO: Assert invarients of clamped between 0 and 1
		Self {
			j,a,b
		}
	}

		fn lms(t: f64) -> f64 {
			if t > 0. {
				let r = t.powf(0.007460772656268214);
				let s = (0.8359375 - r) / (18.6875 * r + -18.8515625);
				s.powf(6.277394636015326)
			} else { 0. }
		}

		fn srgb(c: f64) -> f64 {
			if c < 0.0031308049535603713 {
				c * 12.92
			} else {
				c.powf(0.416_666_666_666_666_7) * 1.055 + -0.055
			}
		}
}

impl Into<Color> for JzAzBzColor {
	#[allow(clippy::many_single_char_names)]
	fn into(self) -> Color {
		let jz = self.j * 0.167_174_631_203_662 + 1.629_549_953_282_156_5e-11;
		let cz = self.a * 0.167_174_631_203_662;
		let hz = self.b * 6.283_185_307_179_586 + -PI;

		let iz = jz / (0.56 * jz + 0.44);
		let az = cz * hz.cos();
		let bz = cz * hz.sin();

		let l = JzAzBzColor::lms(iz + az * 0.138_605_043_271_539_3 + bz * 0.058_047_316_156_118_83);
		let m = JzAzBzColor::lms(iz + az * -0.13860504327153927 + bz * -0.058047316156118904);
		let s = JzAzBzColor::lms(iz + az * -0.09601924202631895 + bz * -0.811_891_896_056_039);

		let lr = l *  5.928_963_755_404_251e2 + m * -5.223_947_425_797_514e2 + s *  3.259_644_233_339_027e1;
		let lg = l * -2.223_295_790_445_722_3e2 + m *  3.821_527_473_694_615e2 + s * -5.703_433_147_128_812e1;
		let lb = l *  6.270_913_830_078_808e0 + m * -7.021_906_556_220_011e1 + s * 1.666_975_603_243_740_8e2;

		let r = scale_u8(JzAzBzColor::srgb(lr));
		let g = scale_u8(JzAzBzColor::srgb(lg));
		let b = scale_u8(JzAzBzColor::srgb(lb));

		Color::RGB(r, g, b)
	}
}

#[cfg(test)]
mod tests {
		use super::*;

		#[test]
		fn test_convert() {
				let jab = JzAzBzColor::new(
					1., 0., 0.);
				let new_color : Color = jab.into();
				assert_eq!(new_color, Color::WHITE);
		}
}
