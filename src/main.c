/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 23:57:54 by pchung            #+#    #+#             */
/*   Updated: 2025/03/12 10:56:04 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

static int	update_exit_status_from_signal(int *exit_stauts)
{
	if (g_signal_received == SIGINT)
		return (130);
	else if (g_signal_received == NOTSIG)
		return (0);
	return (*exit_stauts);
}

static void	print_large_minishell(void)
{
	const char	*header;

	header = MELON_COLOR
		" ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n"
		" ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n"
		" ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n"
		" ██║╚██╔╝██║██║██║ ██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
		" ██║ ╚═╝ ██║██║██║  ████║██║███████║██║  ██║███████╗███████╗███████╗\n\n"
		RESET;
	write(STDOUT_FILENO, header, strlen(header));
}

void	run_shell(t_env *env_list, char **envp)
{
	char	buf[8192];
	size_t	len;
	int		exit_stauts;
	t_ast	*current_ast;

	exit_stauts = 0;
	while (1)
	{
		exit_stauts = update_exit_status_from_signal(&exit_stauts);
		init_readline_for_signal();
		setup_signals();
		ft_memset(buf, 0, sizeof(buf));
		len = 0;
		handle_input(buf, &len, sizeof(buf));
		if (len > 0)
		{
			g_signal_received = RSTSIG;
			current_ast = get_ast(buf, env_list, &exit_stauts);
			exit_stauts = exec_ast(current_ast, env_list, envp);
			free_ast(current_ast);
			ft_memset(buf, 0, sizeof(buf));
			len = 0;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env			*env_list;
	struct termios	orig_termios;

	(void)argc;
	(void)argv;
	env_list = init_env_list(envp);
	setup_terminal(&orig_termios);
	print_large_minishell();
	run_shell(env_list, envp);
	free_env_list(&env_list);
	reset_terminal_settings(&orig_termios);
	return (0);
}
