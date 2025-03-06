#include "executor.h"

void save_fds(int *saved_stdin, int *saved_stdout)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
        perror("save_fds: dup failed");
}

void restore_fds(int saved_stdin, int saved_stdout)
{
    if (dup2(saved_stdin, STDIN_FILENO) == -1)
        perror("restore_fds: dup2 stdin failed");
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
        perror("restore_fds: dup2 stdout failed");

    close(saved_stdin);
    close(saved_stdout);
}

void cleanup_fds(int fd_in, int fd_out)
{
    if (fd_in != -1)
        close(fd_in);
    if (fd_out != -1)
        close(fd_out);
}

void apply_redirections(int fd_in, int fd_out)
{
    if (fd_in != -1)
    {
        if (dup2(fd_in, STDIN_FILENO) == -1) // Redirect input
            perror("dup2 input redirection");
        close(fd_in);
    }
    if (fd_out != -1)
    {
        if (dup2(fd_out, STDOUT_FILENO) == -1) // Redirect output
            perror("dup2 output redirection");
        close(fd_out);
    }
}

int set_filedirectories(t_cmd *cmd, int *fd_in, int *fd_out)
{
    t_list *redirs;
    int exit_status=0;

    if (!cmd) // Handle NULL cmd
        return (1);

    redirs = cmd->redirs;
    *fd_in = -1; // Initialize file descriptors to -1 (unset)
    *fd_out = -1;

    while (redirs)
    {
        t_redir *redir = (t_redir *)redirs->content;

        if (ft_strncmp(redir->type, ">>", 2) == 0) // Append output
        {
            if (!redir->direction || *redir->direction == '\0')
            {
                write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n", 46);
                exit_status=2;
                break;
            }
            if (*fd_out != -1)
                close(*fd_out);

            *fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*fd_out == -1)
            {
                perror("open");
                exit_status=1;
                break;
            }
        }
        else if (ft_strncmp(redir->type, "<<", 2) == 0) 
        {
            if (!redir->direction || *redir->direction == '\0')
            {
                write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n", 46);
                 exit_status=2;
                break;
            }
            if (*fd_in != -1)
                close(*fd_in);

            *fd_in = execute_heredoc(redir->direction);
            if (*fd_in == -1)
            {
                perror("heredoc");
                exit_status=1;
                break;
            }
        }
        else if (ft_strncmp(redir->type, ">", 1) == 0) // Overwrite output
        {
            if (!redir->direction || *redir->direction == '\0')
            {
                write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n", 46);
                 exit_status=2;
                break;
            }
            if (*fd_out != -1)
                close(*fd_out);

            *fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*fd_out == -1)
            {
                perror("open");
                exit_status=1;
                break;
            }
        }
        else if (ft_strncmp(redir->type, "<", 1) == 0) // Input redirection
        {
            if (!redir->direction || *redir->direction == '\0')
            {
                write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n", 46);
                exit_status=2;
                break;
            }
            if (*fd_in != -1)
                close(*fd_in);

            *fd_in = open(redir->direction, O_RDONLY);
            if (*fd_in == -1)
            {
                perror("open");
                exit_status=1;
                break;
            }
        }

        redirs = redirs->next;
    }

    return (exit_status);
}