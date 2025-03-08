/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 23:57:54 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 00:30:15 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void	run_shell(t_env *env_list, char **envp)
{
	int		running;
	char	buf[8192];
	size_t	len;
	int		exit_stauts;
	t_ast	*current_AST;

	running = 1;
	exit_stauts = 0;
	while (running)
	{
		if (g_signal_received == SIGINT)
			exit_stauts = 130;
		else if (g_signal_received == NOTSIG)
			exit_stauts = 0;
		init_readline_for_signal();
		setup_signals();
		ft_memset(buf, 0, sizeof(buf));
		len = 0;
		handle_input(buf, &len, sizeof(buf));
		if (len > 0)
		{
			g_signal_received = RSTSIG;
			current_AST = get_ast(buf, env_list, &exit_stauts);
			exit_stauts = exec_ast(current_AST, env_list, envp);
			free_ast(current_AST);
			ft_memset(buf, 0, sizeof(buf));
			len = 0;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_env *env_list = init_env_list(envp);
	struct termios orig_termios;
	setup_terminal(&orig_termios);
	run_shell(env_list, envp);
	free_env_list(&env_list);
	reset_terminal_settings(&orig_termios);
	return (0);
}