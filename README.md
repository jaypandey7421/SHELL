# Simple Shell in C

This is a simple shell implemented in the C. It supports basic operations such as creating files, deleting files, printing the working directory, and more. The shell is designed to be lightweight and easy to understand, making it a great project for learning about system programming and shell internals.

## Features

- **Create a File**: Create a new file in the current directory.
- **Delete a File**: Delete an existing file from the current directory.
- **Print Working Directory**: Display the current working directory.
- **List Directory Contents**: List all files and directories in the current directory.
- **Change Directory**: Change the current working directory.
- **Execute Commands**: Execute basic shell commands (e.g., `ls`, `echo`, etc.).
- **Exit**: Exit the shell gracefully.

## Getting Started

### Prerequisites

To compile and run this shell, you need:

- A C compiler (e.g., `gcc`)
- A Unix-like operating system (Linux, macOS, etc.)

### Compilation

1. Clone the repository or download the source code.
2. Navigate to the project directory.
3. Compile the shell using the following command:

   ```bash
   gcc -o myshell myshell.c

   $ ./myshell
    myshell> create example.txt
    