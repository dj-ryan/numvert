use clap::{ Arg, Command };
use color_eyre::Result;
use crossterm::event::{ self, Event, KeyCode, KeyEvent, KeyEventKind, KeyModifiers };
use ratatui::{
    DefaultTerminal,
    Frame,
    layout::{ Constraint, Layout },
    style::Stylize,
    text::Line,
    widgets::{ Block, Paragraph },
};

fn main() -> color_eyre::Result<()> {
    color_eyre::install()?;
    // Create a new Command instance for the application
    let matches = Command::new("hex-converter")
        .version("0.1")
        .author("dryan")
        .about("bitwise")
        // Define the argument we want to accept
        .arg(
            Arg::new("input")
                .index(1)
                .help("The number to convert")
                .required(true)
        )
        .get_matches();

    let input_str = matches.get_one::<String>("input").unwrap();

    if input_str.starts_with("0x") {
        let hex_num = u32::from_str_radix(input_str.trim_start_matches("0x"), 16).unwrap();
        println!("Hexadecimal number: {}", hex_num);
    } if input_str.starts_with("0b"){
        let bin_num = u32::from_str_radix(input_str.trim_start_matches("0b"), 2).unwrap();
        println!("Binary number: {}", bin_num);
    }else {
        let num = input_str.parse::<u32>().unwrap();
        println!("Decimal number: {}", num);
    }

    let terminal = ratatui::init();
    let result = App::new().run(terminal);
    ratatui::restore();
    result
}

/// The main application which holds the state and logic of the application.
#[derive(Debug, Default)]
pub struct App {
    /// Is the application running?
    running: bool,
}

impl App {
    /// Construct a new instance of [`App`].
    pub fn new() -> Self {
        Self::default()
    }

    /// Run the application's main loop.
    pub fn run(mut self, mut terminal: DefaultTerminal) -> Result<()> {
        self.running = true;
        while self.running {
            terminal.draw(|frame| self.render(frame))?;
            self.handle_crossterm_events()?;
        }
        Ok(())
    }

    /// Renders the user interface.
    ///
    /// This is where you add new widgets. See the following resources for more information:
    ///
    /// - <https://docs.rs/ratatui/latest/ratatui/widgets/index.html>
    /// - <https://github.com/ratatui/ratatui/tree/main/ratatui-widgets/examples>
    fn render(&mut self, frame: &mut Frame) {
        // layout
        let vertical = Layout::vertical([
            Constraint::Length(1),
            Constraint::Min(0),
            Constraint::Length(1),
        ]);

        // let layout = Layout::
        let [header, main, footer] = vertical.areas(frame.area());
        let horizontil = Layout::horizontal([Constraint::Fill(1); 2]);
        let [left, right] = horizontil.areas(main);

        let title = Line::from("Ratatui Simple Template").bold().blue().centered();
        let text =
            "Hello, Ratatui!\n\n\
            Created using https://github.com/ratatui/templates\n\
            Press `Esc`, `Ctrl-C` or `q` to stop running.";
        // frame.render_widget(
        //     Paragraph::new(text)
        //         .block(Block::bordered().title(title))
        //         .centered(),
        //     frame.area(),
        // )
        frame.render_widget(Block::bordered().title("Title Bar"), header);
        frame.render_widget(Block::bordered().title("Status Bar"), footer);
        frame.render_widget(Block::bordered().title("Left"), left);
        frame.render_widget(Block::bordered().title("Right"), right);
    }

    /// Reads the crossterm events and updates the state of [`App`].
    ///
    /// If your application needs to perform work in between handling events, you can use the
    /// [`event::poll`] function to check if there are any events available with a timeout.
    fn handle_crossterm_events(&mut self) -> Result<()> {
        match event::read()? {
            // it's important to check KeyEventKind::Press to avoid handling key release events
            Event::Key(key) if key.kind == KeyEventKind::Press => self.on_key_event(key),
            Event::Mouse(_) => {}
            Event::Resize(_, _) => {}
            _ => {}
        }
        Ok(())
    }

    /// Handles the key events and updates the state of [`App`].
    fn on_key_event(&mut self, key: KeyEvent) {
        match (key.modifiers, key.code) {
            | (_, KeyCode::Esc | KeyCode::Char('q'))
            | (KeyModifiers::CONTROL, KeyCode::Char('c') | KeyCode::Char('C')) => self.quit(),
            // Add other key handlers here.
            _ => {}
        }
    }

    /// Set running to false to quit the application.
    fn quit(&mut self) {
        self.running = false;
    }
}
