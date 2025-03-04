/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:30:00 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 16:38:55 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../../shell.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

/* AST Execution */
int		exec_ast(t_ast *ast, t_env *env_list);

/* Command Group Execution */
int		exec_command_group(t_command_group *command_group, t_env *env_list);

/* Command Execution */
void	exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int		exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
void	exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);

/* Parent Process Execution */
int		exec_parent(t_list **pids);

/* Built-in Command Checking */
int		is_builtin(t_cmd *cmd);

/* File Descriptor Management */
void	save_fds(int *saved_stdin, int *saved_stdout);
void	restore_fds(int saved_stdin, int saved_stdout);
void	cleanup_fds(int fd_in, int fd_out);
void	apply_redirections(int fd_in, int fd_out);
void	set_filedirectories(t_cmd *cmd, int *fd_in, int *fd_out);

/* Path Resolution */
char	*find_executable_in_path(const char *cmd);
int		has_slash(const char *str);
int		ft_execvp(const char *file, char *const argv[]);

/* Utility Functions */
char	**convert_list_to_arr(t_list *lst);

/* Heredoc Execution */
int		execute_heredoc(const char *delimiter);

#endif
