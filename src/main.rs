use color_eyre::owo_colors::{AnsiColors, OwoColorize, colors::*};
use ratatui::{
    backend::CrosstermBackend,
    // owo_color::{OwoColorize, colors::*},
    buffer::Buffer,
    layout::{Alignment, Constraint, Direction, Layout, Rect},
    prelude::*,
    style::{Color, Style},
    text::{Line, Text},
    widgets::Widget,
    widgets::{Block, Borders, Padding, Paragraph},
};

use crossterm::{
    QueueableCommand,
    style::{Color as CrossColor, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
};

use std::io::{self, Result, Write};

use clap::{Arg, ArgAction, Command, Parser, error::RichFormatter, parser::ValuesRef};

const SMALL_VALUE: f64 = 0.000000000000000000001;

fn parse_input(input: &String) -> u64 {
    let prefix = input.get(0..2);
    match prefix {
        Some("0x") => u64::from_str_radix(&input[2..], 16).unwrap(),
        Some("0b") => u64::from_str_radix(&input[2..], 2).unwrap(),
        _ => input.parse::<u64>().unwrap(),
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
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
    // Define operator types and their corresponding argument names
    let operator_configs = [
        (OPERATOR::plus, "plus"),
        (OPERATOR::minus, "minus"),
        (OPERATOR::times, "times"),
        (OPERATOR::div, "div"),
        (OPERATOR::left_shift, "left-shift"),
        (OPERATOR::right_shift, "right-shift"),
        (OPERATOR::and, "and"),
        (OPERATOR::or, "or"),
    ];

    // Collect values and positions for each operator type
    let mut operator_values = Vec::new();
    let mut all_ops_pos = Vec::new();

    for (op_type, arg_name) in &operator_configs {
        // Get values for this operator type
        if let Some(values) = matches.get_many::<String>(arg_name) {
            let parsed_values: Vec<u64> = values.map(|op| parse_input(op)).collect();

            if !parsed_values.is_empty() {
                operator_values.push((*op_type, parsed_values));
            }
        }

        // Get positions for this operator type
        if let Some(indices) = matches.indices_of(arg_name) {
            let op_positions: Vec<(usize, OPERATOR)> = indices.map(|idx| (idx, *op_type)).collect();

            all_ops_pos.extend(op_positions);
        }
    }

    // Sort operators by their position in the command line
    all_ops_pos.sort_by(|a, b| a.0.cmp(&b.0));

    // Assemble the final list of operations in order
    let mut assembled_math_ops = Vec::with_capacity(all_ops_pos.len());

    for (_, op_type) in all_ops_pos {
        // Find the correct value collection for this operator type
        let value_idx = operator_values.iter_mut().position(|(op, _)| *op == op_type);

        if let Some(idx) = value_idx {
            if let Some(value) = operator_values[idx].1.pop() {
                assembled_math_ops.push((op_type, value));
            } else {
                // No values left for this operator - this shouldn't happen
                // if CLI parsing worked correctly, but handle it gracefully
                return None;
            }
        } else {
            // No values for this operator type - this shouldn't happen
            // if CLI parsing worked correctly, but handle it gracefully
            return None;
        }
    }

    Some(assembled_math_ops)
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
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("plus")
                .short('p')
                .long("plus")
                .aliases(&["sum", "add"])
                .help("Addition operator (+)")
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("times")
                .short('x')
                .visible_short_alias('t')
                .long("times")
                .aliases(&["mult"])
                .help("Multiplication operator (*)")
                .action(ArgAction::Append),
        )
        .arg(Arg::new("div").short('d').long("div").help("Division operator (/)").action(ArgAction::Append))
        .arg(
            Arg::new("left-shift")
                .short('l')
                .long("left-shift")
                .aliases(&["ls"])
                .help("Left shift operator (<<)")
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("right-shift")
                .short('r')
                .long("right-shift")
                .aliases(&["rs", "right-shft"])
                .help("Right shift operator (>>)")
                .action(ArgAction::Append),
        )
        .arg(Arg::new("and").short('a').long("and").help("Bitwise AND operator (&)").action(ArgAction::Append))
        .arg(Arg::new("or").short('o').long("or").help("Bitwise OR operator (|)").action(ArgAction::Append))
        .get_matches();

    let mut input: u64 = parse_input(matches.get_one::<String>("input").unwrap());

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
            let span = Span::styled("1", Style::default().fg(Color::Yellow));
            formated_binary.push_span(span);
        } else {
            let span = Span::styled("0", Style::default().fg(Color::LightMagenta));
            formated_binary.push_span(span);
        }
    }

    let mut formated_binary_discription = Line::default();
    for i in 0..8 {
        formated_binary_discription.push_span(format!("[{:2.}  {:2.}]   ", (8 - i) * 8 - 1, (8 - i) * 8 - 8));
    }

    // Define the area we'll be rendering to
    let area = Rect::new(0, 0, 160, 9);

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
    conversion_text.push_line(Line::from(vec![Span::raw("Dec: "), Span::styled(format!("{}", input), Style::new().red())]));
    conversion_text.push_line(Line::from(vec![Span::raw("Hex: "), Span::styled(format!("0x{:x}", input), Style::default().fg(Color::LightGreen))]));
    conversion_text.push_line(Line::from(vec![Span::raw("Bin: "), Span::styled(format!("0b{:b}", input), Style::default().fg(Color::LightCyan))]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("i3e: "),
        Span::styled(format!("{}", f64::from_bits(input)), Style::default().fg(Color::LightYellow)),
    ]));
    conversion_text.push_line(Line::from(vec![
        Span::raw("2's: "),
        Span::styled(format!("{}", (!input).wrapping_add(1)), Style::default().fg(Color::LightMagenta)),
    ]));
    // OwoColorize::fg(&self);

    let mut byte_breakdown_text = Text::default();
    byte_breakdown_text.push_line(formated_binary);
    byte_breakdown_text.push_line(formated_binary_discription);
    // byte_breakdown_text.centered();

    let sidebar = Paragraph::new(conversion_text).block(Block::default().title("Conversion").borders(Borders::ALL));
    sidebar.render(content_chunks[0], &mut buffer);
    let main_content = Paragraph::new(byte_breakdown_text).block(Block::default().title("Byte breakdown").borders(Borders::ALL).padding(Padding::horizontal(1)));

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
            stdout.queue(SetForegroundColor(fg))?.queue(SetBackgroundColor(bg))?.queue(Print(&cell.symbol()))?;
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

// Test module with conditional compilation
#[cfg(test)]
mod tests {
    use clap::ArgMatches;

    use super::*;

    #[test]
    fn test_add() {
        let mut input = 0x3fce00d1b71758e2;
        let matches = build_match(vec!["numvert", "0x3fce00d1b71758e2", "-p", "10"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x3fce00d1b71758ec);
    }

    #[test]
    fn test_subtract() {
        let mut input = 0x3fce00d1b71758e2;
        let matches = build_match(vec!["numvert", "0x3fce00d1b71758e2", "-m", "10"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x3fce00d1b71758d8);
    }

    #[test]
    fn test_multiply() {
        let mut input = 0x10;
        let matches = build_match(vec!["numvert", "0x10", "-x", "2"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x20);
    }

    #[test]
    fn test_divide() {
        let mut input = 0x20;
        let matches = build_match(vec!["numvert", "0x20", "-d", "2"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x10);
    }

    #[test]
    fn test_left_shift() {
        let mut input = 0x10;
        let matches = build_match(vec!["numvert", "0x10", "-l", "1"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x20);
    }

    #[test]
    fn test_right_shift() {
        let mut input = 0x20;
        let matches = build_match(vec!["numvert", "0x20", "-r", "1"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0x10);
    }

    #[test]
    fn test_and() {
        let mut input = 0xff;
        let matches = build_match(vec!["numvert", "0xff", "-a", "0xf0"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0xf0);
    }

    #[test]
    fn test_or() {
        let mut input = 0xf0;
        let matches = build_match(vec!["numvert", "0xf0", "-o", "0x0f"]);
        let math_ops = assemble_math_ops(&matches);
        input = math_engine(input, math_ops.unwrap());
        assert_eq!(input, 0xff);
    }




    fn build_match(args: Vec<&str>) -> ArgMatches {
        Command::new("numvert")
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
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("plus")
                .short('p')
                .long("plus")
                .aliases(&["sum", "add"])
                .help("Addition operator (+)")
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("times")
                .short('x')
                .visible_short_alias('t')
                .long("times")
                .aliases(&["mult"])
                .help("Multiplication operator (*)")
                .action(ArgAction::Append),
        )
        .arg(Arg::new("div").short('d').long("div").help("Division operator (/)").action(ArgAction::Append))
        .arg(
            Arg::new("left-shift")
                .short('l')
                .long("left-shift")
                .aliases(&["ls"])
                .help("Left shift operator (<<)")
                .action(ArgAction::Append),
        )
        .arg(
            Arg::new("right-shift")
                .short('r')
                .long("right-shift")
                .aliases(&["rs", "right-shft"])
                .help("Right shift operator (>>)")
                .action(ArgAction::Append),
        )
        .arg(Arg::new("and").short('a').long("and").help("Bitwise AND operator (&)").action(ArgAction::Append))
        .arg(Arg::new("or").short('o').long("or").help("Bitwise OR operator (|)").action(ArgAction::Append))
        .get_matches_from(args)
    }


}
