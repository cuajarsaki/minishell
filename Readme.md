# Minishell - As Beautiful as a Shell

Minishell is a minimal shell project written in C, designed to mimic essential functionalities of Bash. Build your own command-line interface while learning about process management, file descriptors, and signal handling.

## Features

- **Interactive Prompt:** Displays a prompt for user input.
- **Command Execution:** Runs commands using the `PATH` variable or relative/absolute paths.
- **Redirections & Pipes:** Supports input/output redirection (`<`, `>`, `<<`, `>>`) and piping (`|`).
- **Environment Variables:** Expands variables (e.g., `$VAR` and `$?` for exit status).
- **Signal Handling:** Properly responds to Ctrl-C, Ctrl-D, and Ctrl-\.

## Built-in Commands

| Command   | Description                                                                              |
|-----------|------------------------------------------------------------------------------------------|
| `echo`    | Prints text to the terminal (supports `-n` option to omit newline).                      |
| `cd`      | Changes the current directory (requires a relative or absolute path).                    |
| `pwd`     | Displays the current working directory.                                                  |
| `export`  | Sets environment variables.                                                              |
| `unset`   | Removes environment variables.                                                           |
| `env`     | Lists all environment variables.                                                         |
| `exit`    | Exits the shell.        

## Final grade
Validated : 09/03/2025. 

Mandatory part : 100/100

Bonus : 0/15

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/minishell.git
   ```
2. **Navigate to the project directory:**
   ```bash
   cd minishell
   ```
3. Compile the project:
   ```bash
    make
   ```
## Usage
- Start the Shell:
  ```bash
  ./minishell
  ```
- Enter commands at the prompt.
- Exit by typing exit or pressing Ctrl-D.
