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
	char	*seperator;
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

typedef struct s_history
{
    char	*command_history[HISTORY_SIZE];
    int		history_start;
    int		history_count;
    int		history_index;
} t_history;

t_ast	*get_ast(const char *input, t_env *env_list);
t_env *init_env_list(void);

// Utility Functions
void run_shell(t_env *env_list);
char *get_env_value(t_env *env_list, const char *key);
void set_env_value(t_env **env_list, const char *key, const char *value);
void unset_env_value(t_env **env_list, const char *key);
void free_env_list(t_env *env_list);
void exec_ast(t_ast *ast, t_env *env_list);
char **convert_env_list_to_array(t_env *env_list);
void free_env_array(char **envp);;
void shell_unknown_command(char *cmd);
void free_ast(t_ast *ast);
void free_cmd(t_cmd *cmd);
void free_command_group(t_command_group *command_group);

// BUILD-IN FUNCTIONS
void shell_env(t_env *env_list);
void shell_export(char **args, t_env *env_list);
void shell_unset(char **args, t_env *env_list);
void shell_clear(void);
void shell_cd(char *path);
void shell_pwd(void);
void shell_echo(char **args);

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

// HISTORY FUNCTIONS
void add_to_history(t_history *h, char *command);
const char *get_history(t_history *h, int direction);
void free_history(t_history *h);
t_history *history_new(void);

#endif
