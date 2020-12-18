use std::error::Error;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::rect::Rect;
use std::time::Duration;

mod jzazbz;
mod settings;

fn main() -> Result<(), Box<dyn Error>> {
    let settings = crate::settings::Settings::load()?;

    let sdl_context = sdl2::init()?;
    let video_context = sdl_context.video()?;
    let controller_context = sdl_context.game_controller()?;
    let controller = controller_context.open(0)?;
    println!("Controller: {}, {}", controller.name(), controller.mapping());

    let window = video_context.window("greeble-demo", 800, 800)
        .position_centered()
        .build()?;

    let mut canvas = window.into_canvas().build()?;
    let bg : sdl2::pixels::Color = settings.ui.colors.background.into();
    let fg : sdl2::pixels::Color = settings.ui.colors.foreground.into();
    canvas.set_draw_color(bg);
    canvas.clear();
    canvas.set_draw_color(fg);
    canvas.fill_rect(Rect::new(10, 10, 50, 50))?;

    canvas.present();

    let mut event_pump = sdl_context.event_pump()?;

    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} |
                Event::KeyDown { keycode: Some(Keycode::Escape), .. } => { break 'running; },
                Event::JoyAxisMotion { axis_idx, value, .. } => { println!("{}={}", axis_idx, value); },
                _ => { },
            }
        }

        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }

    Ok(())
}
