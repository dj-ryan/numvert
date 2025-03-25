use color_eyre::owo_colors::{ colors::*, AnsiColors, OwoColorize };
use ratatui::{
    buffer::Buffer,
    layout::{ Constraint, Direction, Layout, Rect, Alignment },
    style::{ Color, Style },
    widgets::{ Block, Borders, Paragraph, Padding },
    widgets::Widget,
    text::{Line, Text},
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

const SMALL_VALUE: f64 = 0.000000000000000000001;

fn parse_input(input: &String) -> u64 {
    let prefix = input.get(0..2);
    dbg!(prefix);
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

    let mut formated_binary_discription = Line::default();
    for i in 0..8 {

        formated_binary_discription.push_span(format!("[{:2.}  {:2.}]   ", ((8 - i) * 8) - 1, ((8 - i) * 8) - 8));
    }


    // Define the area we'll be rendering to
    let area = Rect::new(0, 0, 200, 9);

    // Create a buffer with the same dimensions as our area
    let mut buffer = Buffer::empty(area);

    // Split the content area into two horizontal chunks
    let content_chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            // Constraint::Percentage(30), // Sidebar
            Constraint::Min(1),
            // Constraint::Percentage(70), // Main content
            Constraint::Max(89)
        ])
        .margin(1)
        .split(area);

    // Create and render a main content block with some text
    let mut conversion_text = Text::default();
    conversion_text.push_line(Line::from(vec![
        Span::raw("Dec: "),
        Span::styled(format!("{}", input), Style::new().red())
    ]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("Hex: "),
        Span::styled(format!("0x{:x}", input), Style::default().fg(Color::LightGreen))
    ]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("Bin: "),
        Span::styled(format!("0b{:b}", input), Style::default().fg(Color::LightCyan))
    ]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("i3e: "),
        Span::styled(format!("{}", f64::from_bits(input)), Style::default().fg(Color::LightYellow))
    ]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("2's: "),
        Span::styled(format!("{}",  (!input).wrapping_add(1)), Style::default().fg(Color::LightMagenta))
    ]));
    // OwoColorize::fg(&self);

    let mut byte_breakdown_text = Text::default();
    byte_breakdown_text.push_line(formated_binary);
    byte_breakdown_text.push_line(formated_binary_discription);
    // byte_breakdown_text.centered();

    let sidebar = Paragraph::new(conversion_text).block(
        Block::default().title("Conversion").borders(Borders::ALL)
    );
    sidebar.render(content_chunks[0], &mut buffer);
    let main_content = Paragraph::new(byte_breakdown_text).block(
        Block::default().title("Byte breakdown").borders(Borders::ALL).padding(Padding::horizontal(1))
    );

    // Render the main content block to the buffer
    main_content.render(content_chunks[1], &mut buffer);
    // byte_breakdown_text.centered().

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
