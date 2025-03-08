/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:30:00 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 02:38:15 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../../shell.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

typedef struct s_exec_ctx
{
	t_command_group	*cmd_group;
	t_env			*env_list;
	char			**envp;
	int				saved_stdin;
	int				saved_stdout;
	int				fd_in;
	int				fd_out;
}					t_exec_ctx;

typedef struct s_pipe_data
{
	int				prev_fd;
	int				i;
	int				cmd_count;
	int				pipe_fd[2];
	t_cmd			*cmd;
	t_command_group	*cg;
	t_env			*env_list;
	char			**envp;
}					t_pipe_data;

/* AST Execution */
int					exec_ast(t_ast *ast, t_env *env_list, char **envp);

/* Command Group Execution */
int					exec_command_group(t_command_group *command_group,
						t_env *env_list, char **envp);

/* Command Execution */
void				exec_cmd(t_cmd *cmd, t_command_group *command_group,
						t_env *env_list, char **envp);
int					exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
int					exec_cmd_external(t_cmd *cmd,
						t_command_group *command_group, t_env *env_list,
						char **envp);

/* Parent Process Execution */
int					exec_parent(t_list **pids);
void				execute_child(t_pipe_data pd);
void				exec_child_process(t_cmd *cmd, t_command_group *cg,
						t_env *env_list, char **envp);
void				setup_pipe_and_fork(t_pipe_data *pd, pid_t *pid);
void				setup_parent_pipe(t_pipe_data *pd);
void				setup_child_pipe(t_pipe_data pd);
void				exec_non_builtin(t_cmd *cmd, t_exec_ctx *ctx);
void				process_builtin(t_cmd *cmd, t_exec_ctx *ctx);
void				handle_cmd_with_path(char *cmd_path, t_exec_ctx *ctx,
						char **tokens);
void				handle_cmd_without_path(char *cmd_path, t_exec_ctx *ctx,
						char **tokens);
int					check_executable_access(char *cmd_path, t_exec_ctx *ctx,
						char **tokens);
void				print_error_and_exit(const char *cmd_path, int exit_status,
						t_exec_ctx *ctx, char **tokens);

/* Built-in Command Checking */
int					is_builtin(t_cmd *cmd);

/* File Descriptor Management */
void				save_fds(int *saved_stdin, int *saved_stdout);
void				restore_fds(int saved_stdin, int saved_stdout);
void				cleanup_fds(int fd_in, int fd_out);
void				apply_redirections(int fd_in, int fd_out);
int					set_filedirectories(t_cmd *cmd, int *fd_in, int *fd_out);

/* Path Resolution */
char				*find_executable_in_path(const char *cmd, t_env *env_list);
int					has_slash(const char *str);
int					ft_execvp(const char *file, char *const argv[], char **envp,
						t_env *env_list);

/* Utility Functions */
char				**convert_list_to_arr(t_list *lst);

/* Heredoc Execution */
int					execute_heredoc(const char *delimiter);

#endif
