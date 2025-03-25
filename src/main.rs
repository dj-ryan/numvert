use color_eyre::owo_colors::{ OwoColorize, colors::*, AnsiColors };
use ratatui::{
    buffer::Buffer,
    layout::{ Constraint, Direction, Layout, Rect },
    style::{ Color, Style },
    widgets::{ Block, Borders, Paragraph },
    widgets::Widget,
    text::Text,
    prelude::*,
    backend::CrosstermBackend,
    // owo_color::{OwoColorize, colors::*},
};

use crossterm::{
    style::{Color as CrossColor, SetBackgroundColor, SetForegroundColor, Print, ResetColor},
    QueueableCommand,
};

use std::io::{ self, Write, Result };

use clap::{ Arg, Command };

fn parse_input(input: &String) -> u64 {
    let prefix = input.get(0..1);
    match prefix {
        Some("0x") => u64::from_str_radix(&input[2..], 16).unwrap(),
        Some("0b") => u64::from_str_radix(&input[2..], 2).unwrap(),
        _ => input.parse::<u64>().unwrap(),
    }
}

fn main() {
    let matches = Command::new("numvert")
        .version("0.1")
        .about("A byte tool")
        .author("daryan")
        // Define the positional argument
        .arg(Arg::new("input").help("The input required for analysis").required(true).index(1))
        .get_matches();

    let input = parse_input(matches.get_one::<String>("input").unwrap());

    let mut input_buffer = input.clone();
    // let mut bin_output_formated = String::from(format!("{:08b}", dec_buf & 0xff));

    let mut formated_binary = Line::default();

    for i in 0..64 {
        let bit = input_buffer >> 63;
        input_buffer <<= 1;
        if i % 8 == 0 && i != 0 {
            formated_binary.push_span(" | ");
        }
        if bit == 1 {
            let span = Span::styled("1", Style::default().fg(Color::LightYellow));
            formated_binary.push_span(span);
        } else {
            let span = Span::styled("0", Style::default().fg(Color::LightMagenta));
            formated_binary.push_span(span);
        }
    }

    // reverse order of the span formated_binary through a closure
    

    // for _ in 0..7 {
    //     bin_output_formated = format!("{:08b} | {}", (dec_buf >> 8) & 0xff, bin_output_formated);
    //     dec_buf >>= 8;
    // }

    // Define the area we'll be rendering to
    let area = Rect::new(0, 0, 130, 7);

    // Create a buffer with the same dimensions as our area
    let mut buffer = Buffer::empty(area);

    // Split the content area into two horizontal chunks
    let content_chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            // Constraint::Percentage(30), // Sidebar
            Constraint::Min(1),
            // Constraint::Percentage(70), // Main content
            Constraint::Max(87)
        ])
        .margin(1)
        .split(area);

    // Create and render a main content block with some text
    let conversion_text = Text::from(
        format!("Dec: {}\nHex: 0x{:x}\nBin: 0b{:b}", input, input, input)
    );

    // OwoColorize::fg(&self);

    let byte_breakdown_text = Text::from(formated_binary);

    let sidebar = Paragraph::new(conversion_text).block(
        Block::default().title("Conversion").borders(Borders::ALL)
    );
    sidebar.render(content_chunks[0], &mut buffer);
    let main_content = Paragraph::new(byte_breakdown_text).block(
        Block::default().title("Byte breakdown").borders(Borders::ALL)
    );

    // Render the main content block to the buffer
    main_content.render(content_chunks[1], &mut buffer);

    // Print the buffer content to stdout
    let _ = print_buffer(&buffer);
}

// Function to print the buffer to stdout
fn print_buffer(buffer: &Buffer) -> Result<()> {
    // Convert the buffer to a string representation
    // let mut output = String::new();

    // let backend = CrosstermBackend::new(stdout());
    // backend.style(style)

    let mut stdout = io::stdout();

    // Write the buffer content to stdout with styling
    for y in 0..buffer.area.height {
        for x in 0..buffer.area.width {
            let cell = buffer.get(x, y);

            // Convert ratatui colors to crossterm colors
            let fg = convert_color(cell.fg);
            let bg = convert_color(cell.bg);

            // Apply styling and print the symbol
            stdout
                .queue(SetForegroundColor(fg))?
                .queue(SetBackgroundColor(bg))?
                .queue(Print(&cell.symbol()))?;
        }
        stdout.queue(ResetColor)?.queue(Print("\n"))?;
    }
    stdout.flush()?;


    // Reset any styling
    // backend.style(Style::default())?;
    println!();

    // Print the output
    // println!("{}", output);
    Ok(())
}

// Helper function to convert ratatui colors to crossterm colors
fn convert_color(color: Color) -> CrossColor {
    match color {
        Color::Black => CrossColor::Black,
        Color::Red => CrossColor::DarkRed,
        Color::Green => CrossColor::DarkGreen,
        Color::Yellow => CrossColor::DarkYellow,
        Color::Blue => CrossColor::DarkBlue,
        Color::Magenta => CrossColor::DarkMagenta,
        Color::Cyan => CrossColor::DarkCyan,
        Color::Gray => CrossColor::Grey,
        Color::DarkGray => CrossColor::DarkGrey,
        Color::LightRed => CrossColor::Red,
        Color::LightGreen => CrossColor::Green,
        Color::LightYellow => CrossColor::Yellow,
        Color::LightBlue => CrossColor::Blue,
        Color::LightMagenta => CrossColor::Magenta,
        Color::LightCyan => CrossColor::Cyan,
        Color::White => CrossColor::White,
        Color::Rgb(r, g, b) => CrossColor::Rgb { r, g, b },
        Color::Indexed(i) => CrossColor::AnsiValue(i),
        _ => CrossColor::Reset,
    }
}

