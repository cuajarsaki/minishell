#include "shell.h"

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}


void set_prompt(const char *prompt, t_env *env_list)
{
    int running = 1;
    char buf[8192];
    size_t len;

    term_clear_screen();
    while (running) {
        write(STDOUT_FILENO, prompt, ft_strlen(prompt));
        ft_memset(buf, 0, sizeof(buf));
        len = 0;
        handle_input(buf, &len, sizeof(buf));

        if (ft_strcmp(buf, "exit") == 0) {
            running = 0;
            continue;
        }
        if (len > 0) {
            process_command(buf, env_list); // Pass env_list to process_command
            ft_memset(buf, 0, sizeof(buf));
            len = 0;
        }
    }
}

int main(void)
{
    t_env *env_list = init_env_list(); // Initialize environment variables

    setup_terminal();
    ignore_signals();
    set_prompt("minishell ❤ ", env_list); // Pass env_list

    free_env_list(env_list); // Free environment variables before exit
    free_history();
    reset_terminal_settings();
    
    return 0;
}