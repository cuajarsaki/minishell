English | [日本語](./docs/Readme-ja.md) | [繁體中文](./docs/Readme-zh.md)


# Minishell - As Beautiful as a Shell
<p align="left">
  <img src="https://img.shields.io/badge/-C-213a70.svg?logo=C&style=flat">
  <img src="https://img.shields.io/badge/-Linux-ea5520.svg?logo=linux&style=flat">
  <img src="https://img.shields.io/badge/Subject_Version-8.02-3E72BC.svg">
  <img src="https://github.com/cuajarsaki/minishell/actions/workflows/test.yml/badge.svg" alt="minishell">
</p>
<p align="center">
  <img src="https://github.com/cuajarsaki/minishell/blob/a5fad336ac54386db5af09fb4ca2ee568a382864/docs/minishell_icon.png" alt="Minishell 42 project badge" style="width:10%;"/>
</p>
Minishell is a mini shell project written in C that mimics the basic functionalities of Bash.
While learning about process management, file descriptors, and signal handling, We built our own command-line interface.
We paid special attention to accurately replicating bash's behavior regarding exit status values and error messages, ensuring that our shell responds to errors in a consistent and familiar way for users accustomed to bash.

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
| `cd`      | Changes the current directory                                                            |
| `pwd`     | Displays the current working directory.                                                  |
| `export`  | Sets environment variables.                                                              |
| `unset`   | Removes environment variables.                                                           |
| `env`     | Lists all environment variables.                                                         |
| `exit`    | Exits the shell.        

## Norminette Compliance
This project strictly adheres to the 42 School Norminette rules (v3.3.55). The Norminette enforces a consistent coding style across all C projects at 42, including:

- 25 lines maximum per function
- 80 characters maximum per line
- Only 5 functions maximum per file
- Specific variable declaration and naming conventions
- Restricted use of standard library functions
- Structured function header format
- No for loops, switch statements, or do-while loops
- Required use of tabs for indentation

All code in this repository passes the official 42 Norminette validator without errors.

><img src="https://github.com/cuajarsaki/minishell/blob/a5fad336ac54386db5af09fb4ca2ee568a382864/docs/norminette.png" alt="grade" style="width:40%;"/>

## Final grade
<img src="https://github.com/user-attachments/assets/765ec3c2-7927-4a92-b718-ec39fc93c64d" alt="grade" style="width:20%;"/>

Validated : 09/03/2025

Mandatory part : 100/100

Bonus : 0/15


## Flowchart of Main

```mermaid
  
flowchart LR
    A[Start main] --> B[Init env_list] --> C[Setup terminal] --> D[run_shell] --> E[Cleanup] --> F[Exit]
    
    D --> G[run_shell loop]
    G --> H[Handle signals] --> I[Get input] --> J{any Input?}
    J -->|No| G
    J -->|Yes| K[parse_ast] --> L[exec_ast] --> M[free_ast] --> G
```

## Flowchart of Execution Part
```mermaid

graph TD
    A[exec_ast] --> B[exec_command_group]
    B --> C{is no command?}
    C -->|Yes| D[return 0]
    C -->|No| E{is single command?}
    E -->|Yes| F{is exit/cd/unset/export?}
    F -->|Yes| G[exec_cmd_builtin]
    F -->|No| H[fork]
    H -->|Child| I[exec_child_process]
    H -->|Parent| J[exec_parent]
    E -->|No| K[process_command]
    K --> L[setup_pipe_and_fork]
    L --> M[fork]
    M -->|Child| N[execute_child]
    M -->|Parent| O[setup_parent_pipe]
    K --> P[exec_parent]
    B --> Q[free_command_group]
    A --> R[return exit_status]
    I --> S[exec_cmd]
    N --> S
    S --> T[set_filedirectories]
    T --> U[handle_append_output]
    T --> V[handle_heredoc]
    T --> W[handle_overwrite_output]
    T --> X[handle_input_redir]
    S --> Y[save_fds]
    S --> Z[apply_redirections]
    S --> AA[process_command]
    AA --> AB{is builtin?}
    AB -->|Yes| AC[process_builtin]
    AB -->|No| AD[exec_non_builtin]
    AC --> AE[exec_cmd_builtin]
    AD --> AF[handle_cmd_with_path]
    AD --> AG[handle_cmd_without_path]
    AF --> AH[check_executable_access]
    AG --> AI[find_executable_in_path]
    AI --> AJ[search_executable_in_paths]
    AD --> AK[exec_cmd_external]
    AK --> AL[ft_execvp]
    AL --> AM[try_execve_in_paths]
    AL --> AN[execve]
    AK --> AO[perror]
    AE --> AP[shell_echo]
    AE --> AQ[shell_cd]
    AE --> AR[shell_exit]
    AE --> AS[shell_pwd]
    AE --> AT[shell_export]
    AE --> AU[shell_unset]
    AE --> AV[shell_env]
    V --> AW[execute_heredoc]
    AW --> AX[heredoc_loop]
    U --> AY[open]
    W --> AZ[open]
    X --> BA[open]
    Z --> BB[dup2]
    Y --> BC[dup]
    BB --> BD[close]
    BC --> BE[close]
    J --> BF[waitpid]
    O --> BG[close]
    L --> BH[pipe]
    BH --> BI[perror]
    BI --> BJ[exit]

    subgraph "Initialization and Execution"
        A[exec_ast]
        B[exec_command_group]
        C{is no command?}
        D[return 0]
        E{is single command?}
        F{is exit/cd/unset/export?}
        G[exec_cmd_builtin]
        H[fork]
        I[exec_child_process]
        J[exec_parent]
        Q[free_command_group]
        R[return exit_status]
    end

    subgraph "Command Execution"
        K[process_command]
        L[setup_pipe_and_fork]
        M[fork]
        N[execute_child]
        O[setup_parent_pipe]
        P[exec_parent]
        S[exec_cmd]
        T[set_filedirectories]
        U[handle_append_output]
        V[handle_heredoc]
        W[handle_overwrite_output]
        X[handle_input_redir]
        Y[save_fds]
        Z[apply_redirections]
        AA[process_command]
        AB{is builtin?}
        AC[process_builtin]
        AD[exec_non_builtin]
        AE[exec_cmd_builtin]
        AF[handle_cmd_with_path]
        AG[handle_cmd_without_path]
        AH[check_executable_access]
        AI[find_executable_in_path]
        AJ[search_executable_in_paths]
        AK[exec_cmd_external]
        AL[ft_execvp]
        AM[try_execve_in_paths]
        AN[execve]
        AO[perror]
        AP[shell_echo]
        AQ[shell_cd]
        AR[shell_exit]
        AS[shell_pwd]
        AT[shell_export]
        AU[shell_unset]
        AV[shell_env]
        AW[execute_heredoc]
        AX[heredoc_loop]
        AY[open]
        AZ[open]
        BA[open]
        BB[dup2]
        BC[dup]
        BD[close]
        BE[close]
        BF[waitpid]
        BG[close]
        BH[pipe]
        BI[perror]
        BJ[exit]
    end



```

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


