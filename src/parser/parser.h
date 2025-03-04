/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:00:00 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 16:10:23 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../../shell.h"
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdio.h>

/* ************************************************************************** */
/*                           UTILITY FUNCTIONS                                */
/* ************************************************************************** */

void	ft_skip_spaces(const char *input, int *curr_pos);
char	*ft_realloc_str(char *ptr, size_t new_size);

/* ************************************************************************** */
/*                           ENVIRONMENT EXPANSION                            */
/* ************************************************************************** */

char	*expand_env_token(const char *token, t_env *env_list, int *exit_status);

/* ************************************************************************** */
/*                           TOKEN UTILITIES                                  */
/* ************************************************************************** */

char	*remove_quotes(const char *token);

/* ************************************************************************** */
/*                           REDIRECTION PARSING                              */
/* ************************************************************************** */

// typedef struct s_redir
// {
// 	char	type[3];
// 	char	*direction;
// }	t_redir;

t_redir	*get_redir(const char *input, int *curr_pos);

/* ************************************************************************** */
/*                           TOKEN PARSING                                    */
/* ************************************************************************** */

// typedef struct s_token
// {
// 	char	*value;
// 	int		is_single_quoted;
// }	t_token;

t_token	*get_token(const char *input, int *curr_pos);

/* ************************************************************************** */
/*                           COMMAND PARSING                                  */
/* ************************************************************************** */

// typedef struct s_cmd
// {
// 	t_list	*tokens;
// 	t_list	*redirs;
// }	t_cmd;

t_cmd	*get_cmd(const char *input, int *curr_pos, t_env *env_list, int *exit_status);

/* ************************************************************************** */
/*                           COMMAND GROUP PARSING                            */
/* ************************************************************************** */

// typedef struct s_command_group
// {
// 	t_list	*cmds;
// }	t_command_group;

t_command_group	*get_command_group(const char *input, int *curr_pos, t_env *env_list, int *exit_status);

/* ************************************************************************** */
/*                           AST PARSING                                      */
/* ************************************************************************** */

// typedef struct s_ast
// {
// 	t_list	*command_groups;
// }	t_ast;

t_ast	*get_ast(const char *input, t_env *env_list, int *exit_status);

#endif
