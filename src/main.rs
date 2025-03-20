use ratatui::{
    buffer::Buffer,
    layout::{Constraint, Direction, Layout, Rect},
    style::{Color, Style},
    widgets::{Block, Borders, Paragraph},
    widgets::Widget,
};

fn main() {
    // Define the area we'll be rendering to
    let area = Rect::new(0, 0, 80, 24);
    
    // Create a buffer with the same dimensions as our area
    let mut buffer = Buffer::empty(area);
    
    // Create a layout with two vertical chunks
    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .margin(1)
        .constraints([
            Constraint::Length(3),  // Header area
            Constraint::Min(0),     // Content area
        ])
        .split(area);
    
    // Create and render a header block
    let header = Block::default()
        .title("Header")
        .borders(Borders::ALL)
        .style(Style::default().fg(Color::White).bg(Color::Blue));
    
    // Render the header block to the buffer
    header.render(chunks[0], &mut buffer);
    
    // Split the content area into two horizontal chunks
    let content_chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            Constraint::Percentage(30), // Sidebar
            Constraint::Percentage(70), // Main content
        ])
        .split(chunks[1]);
    
    // Create and render a sidebar block
    let sidebar = Block::default()
        .title("Sidebar")
        .borders(Borders::ALL);
    
    // Render the sidebar block to the buffer
    sidebar.render(content_chunks[0], &mut buffer);
    
    // Create and render a main content block with some text
    let main_content = Paragraph::new("This is the main content area.")
        .block(Block::default().title("Main Content").borders(Borders::ALL));
    
    // Render the main content block to the buffer
    main_content.render(content_chunks[1], &mut buffer);
    
    // Print the buffer content to stdout
    print_buffer(&buffer);
}

// Function to print the buffer to stdout
fn print_buffer(buffer: &Buffer) {
    // Convert the buffer to a string representation
    let mut output = String::new();
    
    // Get buffer content as a vector of lines
    let content = buffer.content.iter().map(|cell| cell.symbol()).collect::<Vec<_>>();
    
    // Iterate through each line of the buffer
    for y in 0..buffer.area.height {
        let mut line = String::new();
        
        // Iterate through each cell in the line
        for x in 0..buffer.area.width {
            let idx = (y * buffer.area.width + x) as usize;
            if idx < content.len() {
                line.push_str(&content[idx]);
            } else {
                line.push(' ');
            }
        }
        
        // Add the line to the output
        output.push_str(&line);
        output.push('\n');
    }
    
    // Print the output
    println!("{}", output);
}