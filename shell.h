#ifndef SHELL_H
#define SHELL_H

#include "libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <term.h>
#include <termios.h>

#define HISTORY_SIZE 10

extern volatile sig_atomic_t g_signal_received;

typedef struct s_ast
{
	t_list	*command_groups;
} t_ast;

typedef struct s_command_group
{
	t_list	*cmds;
	int		cmd_amount;
	int		**pipes;
	t_list	*pids;
	int		return_value;
} t_command_group;

typedef struct s_cmd
{
	t_list	*tokens;
	t_list	*redirs;
} t_cmd;

typedef struct s_redir
{
	char	*direction;
	char	type[2];
} t_redir;

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef struct s_token
{
	char	*value;
	int		is_single_quoted;
}	t_token;


t_ast	*get_ast(const char *input, t_env *env_list, int *exit_status);
t_env *init_env_list(char ** environ);

// Utility Functions
void run_shell(t_env *env_list, char **envp);
char *get_env_value(t_env *env_list, const char *key);
void set_env_value(t_env **env_list, const char *key, const char *value);
void unset_env_value(t_env **env_list, const char *key);
void free_env_list(t_env *env_list);
int exec_ast(t_ast *ast, t_env *env_list, char **envp);
char **convert_env_list_to_array(t_env *env_list);
void free_env_array(char **envp);;
void shell_unknown_command(char *cmd);

// FREE FUNCTIONS
void free_ast(t_ast *ast);
void free_cmd(t_cmd *cmd);
void free_command_group(t_command_group *command_group);
void free_redir(void *ptr);
void free_argv(char **argv);

// BUILD-IN FUNCTIONS
int shell_env(t_env *env_list);
int shell_export(char **args, t_env *env_list);
int shell_unset(char **args, t_env *env_list);
int shell_clear(void);
int shell_cd(char **argv);
int shell_pwd(void);
int shell_echo(char **args);
int shell_exit(char **argv);

// TERMINAL SETTINGS
void reset_terminal_settings(const struct termios *old_termios);
void setup_terminal(struct termios *old_termios);
void term_clear_screen();

// TERMCAP FUNCTIONS
void handle_input(char *buf, size_t *len, size_t max_len);

// SINGAL  FUNCTIONS
void init_readline_for_signal(void);
void setup_signals(void);
void handle_sigint(int sig);
void set_signal(int signum, void (*handler)(int), int flags);
void init_signal(void (*handler_for_sigint)(int),
				 void (*handler_for_sigquit)(int));

// PARSER FUNCTIONS
char	**token_list_to_argv(t_cmd *cmd);

#endif
