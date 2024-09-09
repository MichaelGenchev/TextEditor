# C++ Text Editor

A powerful and efficient text editor implemented in C++, utilizing the Rope data structure for optimal text manipulation.

## Features

- **Rope Data Structure**: Efficient for large text documents
- **Cursor Navigation**: Move cursor up, down, left, and right with preferred column feature
- **Text Manipulation**: Insert, delete, and replace text
- **Undo/Redo**: Full support for undoing and redoing actions
- **File Operations**: Open and save files
- **Search Functionality**: Find text within the document
- **Command-Line Interface**: Easy-to-use commands for all operations

## Getting Started

### Prerequisites

- C++ compiler with C++11 support or later
- Make (optional, for building)

### Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/cpp-text-editor.git
   cd cpp-text-editor
   ```

2. Compile the project:
   ```
   g++ -std=c++11 -o text_editor main.cpp text_editor.cpp
   ```

### Running the Editor

Run the compiled executable:

```
./text_editor
```

## Usage

Once the editor is running, you can use the following commands:

- `i <text>` - Insert text at cursor
- `d <n>` - Delete n characters from cursor
- `m <row> <col>` - Move cursor to specified position
- `g <line>` - Go to specified line
- `p` - Print current text
- `u` - Undo last action
- `r` - Redo last undone action
- `f <text>` - Find text in the document
- `s <old> <new>` - Replace text
- `o <filename>` - Open file
- `w <filename>` - Write to file
- `q` - Quit the editor
- `h` - Show help menu

## Implementation Details

### Rope Data Structure

The editor uses a Rope data structure to store and manipulate text efficiently. This allows for faster insertion and deletion operations, especially for large documents.

### Cursor Class

The Cursor class manages the current position within the text, supporting movement in all directions and maintaining a preferred column for vertical movement.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Thanks to all contributors who have helped with this project.
- Inspired by classic text editors and modern data structures.