use color_eyre::owo_colors::{OwoColorize, colors::*, AnsiColors};
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

use std::io::{stdout, Write};

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

    for i in 0..63 {
        let bit = input_buffer & 0x1; // capture last bit
        input_buffer >>= 1;
        if bit == 1 {
            let span = Span::styled("1", Style::default().fg(Color::Yellow));
            formated_binary.push_span(span);
        } else {
            let span = Span::styled("0", Style::default().fg(Color::Red));
            formated_binary.push_span(span);
        }        
        if i % 8 == 0 {
            formated_binary.push_span(" | ");
        } 
    }


    // for _ in 0..7 {
    //     bin_output_formated = format!("{:08b} | {}", (dec_buf >> 8) & 0xff, bin_output_formated);
    //     dec_buf >>= 8;
    // }

    // Define the area we'll be rendering to
    let area = Rect::new(0, 0, 140, 7);

    // Create a buffer with the same dimensions as our area
    let mut buffer = Buffer::empty(area);

    // Split the content area into two horizontal chunks
    let content_chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            Constraint::Percentage(30), // Sidebar
            Constraint::Percentage(70), // Main content
        ])
        .margin(1)
        .split(area);


    // Create and render a main content block with some text
    let conversion_text = Text::from(
        format!(
            "Dec: {}\nHex: 0x{:x}\nBin: 0b{:b}",
            input,
            input,
            input
        )
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
    print_buffer(&buffer);
}

// Function to print the buffer to stdout
fn print_buffer(buffer: &Buffer) {
    // Convert the buffer to a string representation
    // let mut output = String::new();

    let backend = CrosstermBackend::new(stdout());

    // backend.style(style)

    // Write the buffer content to stdout with styling
    for (idx, cell) in buffer.content.iter().enumerate() {
        let x = idx as u16 % buffer.area.width;
        let y = idx as u16 / buffer.area.width;
        
        if cell.style(). {
            // Only print non-empty cells
            if !cell.symbol().is_empty() && cell.symbol() != " " {
                backend.style(cell.style());
                print!("{}", cell.symbol());
                backend.style(Style::default()); // Reset style
            } else {
                print!(" ");
            }
        } else {
            print!("{}", cell.symbol());
        }
        
        // Add newline at the end of each row
        if x == buffer.area.width - 1 {
            println!();
        }
    }
    
    // Reset any styling
    backend.style(Style::default())?;
    println!();

    // Print the output
    // println!("{}", output);
}
