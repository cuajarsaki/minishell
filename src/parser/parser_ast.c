/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:09:23 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 19:08:10 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*get_ast(const char *input, t_env *env_list, int *exit_status)
{
	t_ast	*ast;
	t_list	*command_group;
	int		curr_pos;
	void	*cmd_grp;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->command_groups = NULL;
	curr_pos = 0;
	while (input[curr_pos])
	{
		ft_skip_spaces(input, &curr_pos);
		cmd_grp = (void *)get_command_group(input, &curr_pos, env_list,
				exit_status);
		command_group = ft_lstnew(cmd_grp);
		if (!command_group)
		{
			free(ast);
			return (NULL);
		}
		ft_lstadd_back(&ast->command_groups, command_group);
	}
	return (ast);
}
