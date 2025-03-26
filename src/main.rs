use color_eyre::owo_colors::{ colors::*, AnsiColors, OwoColorize };
use ratatui::{
    buffer::Buffer,
    layout::{ Constraint, Direction, Layout, Rect, Alignment },
    style::{ Color, Style },
    widgets::{ Block, Borders, Paragraph, Padding },
    widgets::Widget,
    text::{ Line, Text },
    prelude::*,
    backend::CrosstermBackend,
    // owo_color::{OwoColorize, colors::*},
};

use crossterm::{
    style::{ Color as CrossColor, SetBackgroundColor, SetForegroundColor, Print, ResetColor },
    QueueableCommand,
};

use std::io::{ self, Write, Result };

use clap::{ error::RichFormatter, Arg, ArgAction, Command, Parser };

const SMALL_VALUE: f64 = 0.000000000000000000001;

fn parse_input(input: &String) -> u64 {
    let prefix = input.get(0..2);
    match prefix {
        Some("0x") => u64::from_str_radix(&input[2..], 16).unwrap(),
        Some("0b") => u64::from_str_radix(&input[2..], 2).unwrap(),
        _ => input.parse::<u64>().unwrap(),
    }
}

#[derive(Debug, Clone, Copy)]
enum OPERATOR {
    plus,
    minus,
    times,
    div,
    left_shift,
    right_shift,
    and,
    or,
    // xor,
}

// struct math_flags

fn execute_op(input: u64, op: OPERATOR, value: u64) -> u64 {
    match op {
        OPERATOR::plus => input + value,
        OPERATOR::minus => input - value,
        OPERATOR::times => input * value,
        OPERATOR::div => input / value,
        OPERATOR::left_shift => input << value,
        OPERATOR::right_shift => input >> value,
        OPERATOR::and => input & value,
        OPERATOR::or => input | value,
        // OPERATOR::xor => input ^ value,
    }
}

fn math_engine(input: u64, operations: Vec<(OPERATOR, u64)>) -> u64 {
    let mut result = input;
    for (op, value) in operations {
        result = execute_op(result, op.clone(), value);
        dbg!(op, value, result);
    }
    result
}

fn assemble_math_ops(matches: &clap::ArgMatches) -> Option<Vec<(OPERATOR, u64)>> {
    let mut add_ops: Vec<u64> = matches
        .get_many::<String>("plus")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut sub_ops: Vec<u64> = matches
        .get_many::<String>("minus")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut mult_ops: Vec<u64> = matches
        .get_many::<String>("times")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut div_ops: Vec<u64> = matches
        .get_many::<String>("div")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut left_shift_ops: Vec<u64> = matches
        .get_many::<String>("left-shift")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut right_shift_ops: Vec<u64> = matches
        .get_many::<String>("right-shift")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut and_ops: Vec<u64> = matches
        .get_many::<String>("and")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();
    let mut or_ops: Vec<u64> = matches
        .get_many::<String>("or")
        .unwrap()
        .map(|op_val| op_val.parse::<u64>().unwrap())
        .collect();

    let add_ops_idx = matches.indices_of("plus");
    let sub_ops_idx = matches.indices_of("minus");
    let mult_ops_idx = matches.indices_of("times");
    let div_ops_idx = matches.indices_of("div");
    let left_shift_ops_idx = matches.indices_of("left-shift");
    let right_shift_ops_idx = matches.indices_of("right-shift");
    let and_ops_idx = matches.indices_of("and");
    let or_ops_idx = matches.indices_of("or");


    let mut all_ops_pos: Vec<(usize, OPERATOR)> = Vec::new();
    all_ops_pos.extend(add_ops_idx.unwrap().map(|op| (op, OPERATOR::plus)));
    all_ops_pos.extend(sub_ops_idx.unwrap().map(|op| (op, OPERATOR::minus)));
    all_ops_pos.extend(mult_ops_idx.unwrap().map(|op| (op, OPERATOR::times)));
    all_ops_pos.extend(div_ops_idx.unwrap().map(|op| (op, OPERATOR::div)));
    all_ops_pos.extend(left_shift_ops_idx.unwrap().map(|op| (op, OPERATOR::left_shift)));
    all_ops_pos.extend(right_shift_ops_idx.unwrap().map(|op| (op, OPERATOR::right_shift)));
    all_ops_pos.extend(and_ops_idx.unwrap().map(|op| (op, OPERATOR::and)));
    all_ops_pos.extend(or_ops_idx.unwrap().map(|op| (op, OPERATOR::or)));

    all_ops_pos.sort_by(|a, b| a.0.cmp(&b.0));

    let mut assembled_math_ops: Vec<(OPERATOR, u64)> = Vec::new();
    for op in all_ops_pos.iter().enumerate() {
        let mut add_idx = 0;
        let mut sub_idx = 0;
        let mut mult_idx = 0;
        let mut div_idx = 0;
        let mut left_shift_idx = 0;
        let mut right_shift_idx = 0;
        let mut and_idx = 0;
        let mut or_idx = 0;
        
        let next_op = match op.1.1 {
            OPERATOR::plus => {
                let val = add_ops[add_idx];
                add_idx += 1;
                val
            },
            OPERATOR::minus => {
                let val = sub_ops[sub_idx];
                sub_idx += 1;
                val
            },
            OPERATOR::times => {
                let val = mult_ops[mult_idx];
                mult_idx += 1;
                val
            },
            OPERATOR::div => {
                let val = div_ops[div_idx];
                div_idx += 1;
                val
            },
            OPERATOR::left_shift => {
                let val = left_shift_ops[left_shift_idx];
                left_shift_idx += 1;
                val
            },
            OPERATOR::right_shift => {
                let val = right_shift_ops[right_shift_idx];
                right_shift_idx += 1;
                val
            },
            OPERATOR::and => {
                let val = and_ops[and_idx];
                and_idx += 1;
                val
            },
            OPERATOR::or => {
                let val = or_ops[or_idx];
                or_idx += 1;
                val
            },
        };
        assembled_math_ops.push((op.1.1, next_op));
    }

    return Some(assembled_math_ops);
}

fn main() {
    let matches = Command::new("numvert")
        .version("0.1")
        .about("A byte tool")
        .author("daryan")
        // Define the positional argument
        .arg(Arg::new("input").help("The input required for analysis").required(true).index(1))
        .arg(
            Arg::new("minus")
                .short('m')
                .long("minus")
                .aliases(&["dif", "sub"])
                .help("Subtraction operator (-)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("plus")
                .short('p')
                .long("plus")
                .aliases(&["sum", "add"])
                .help("Addition operator (+)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("times")
                .short('x')
                .visible_short_alias('t')
                .long("times")
                .aliases(&["mult"])
                .help("Multiplication operator (*)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("div")
                .short('d')
                .long("div")
                .help("Division operator (/)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("left-shift")
                .short('l')
                .long("left-shift")
                .aliases(&["ls"])
                .help("Left shift operator (<<)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("right-shift")
                .short('r')
                .long("right-shift")
                .aliases(&["rs", "right-shft"])
                .help("Right shift operator (>>)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("and")
                .short('a')
                .long("and")
                .help("Bitwise AND operator (&)")
                .action(ArgAction::Append)
        )
        .arg(
            Arg::new("or")
                .short('o')
                .long("or")
                .help("Bitwise OR operator (|)")
                .action(ArgAction::Append)
        )
        .get_matches();

    let mut input = parse_input(matches.get_one::<String>("input").unwrap());

    // let values = matches.remove_many(id)

    // let args: Vec<String> = std::env::args().collect();
    // for arg in args {
    //     println!("{}", arg);
    // }

    let math_ops = assemble_math_ops(&matches);
    input = math_engine(input, math_ops.unwrap());

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
        formated_binary_discription.push_span(
            format!("[{:2.}  {:2.}]   ", (8 - i) * 8 - 1, (8 - i) * 8 - 8)
        );
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
            Constraint::Max(89),
        ])
        .margin(1)
        .split(area);

    // Create and render a main content block with some text
    let mut conversion_text = Text::default();
    conversion_text.push_line(
        Line::from(vec![Span::raw("Dec: "), Span::styled(format!("{}", input), Style::new().red())])
    );
    conversion_text.push_line(
        Line::from(
            vec![
                Span::raw("Hex: "),
                Span::styled(format!("0x{:x}", input), Style::default().fg(Color::LightGreen))
            ]
        )
    );
    conversion_text.push_line(
        Line::from(
            vec![
                Span::raw("Bin: "),
                Span::styled(format!("0b{:b}", input), Style::default().fg(Color::LightCyan))
            ]
        )
    );
    conversion_text.push_line(
        Line::from(
            vec![
                Span::raw("i3e: "),
                Span::styled(
                    format!("{}", f64::from_bits(input)),
                    Style::default().fg(Color::LightYellow)
                )
            ]
        )
    );
    conversion_text.push_line(
        Line::from(
            vec![
                Span::raw("2's: "),
                Span::styled(
                    format!("{}", (!input).wrapping_add(1)),
                    Style::default().fg(Color::LightMagenta)
                )
            ]
        )
    );
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
        Block::default()
            .title("Byte breakdown")
            .borders(Borders::ALL)
            .padding(Padding::horizontal(1))
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
