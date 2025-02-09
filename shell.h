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

t_ast	*get_ast(const char *input, t_env *env_list);

void exec_ast(t_ast *ast, t_env *env_list);

//OLD CODE

void shell_env(t_env *env_list);
void shell_export(char **args, t_env *env_list);
void shell_unset(char **args, t_env *env_list);





typedef struct {
	char **tokens;
	size_t count;
} LexerBuffer;

// History constants
#define HISTORY_SIZE 10
// History variables
extern char *command_history[HISTORY_SIZE];
extern int history_start;
extern int history_count;
extern int history_index;



t_env *init_env_list(void);
char *get_env_value(t_env *env_list, const char *key);
void set_env_value(t_env **env_list, const char *key, const char *value);
void unset_env_value(t_env **env_list, const char *key);
void print_env_list(t_env *env_list);
void free_env_list(t_env *env_list);

// Utility Functions
char **convert_env_list_to_array(t_env *env_list);
void free_env_array(char **envp);
void add_to_history(char *command);
const char *get_history(int direction);

void ignore_signals(void);
void set_prompt(const char *prompt, t_env *env_list);
void process_command(char *input, t_env *env_list);
void shell_clear(void);
void shell_cd(char *path);
void shell_pwd(void);
void shell_unknown_command(char *cmd);
void lexer_build(char *input, LexerBuffer *lexerbuf);
void lexer_free(LexerBuffer *lexerbuf);
void	shell_echo(char **args);

//TERMCAP FUNCTIONS
void reset_terminal_settings();
void setup_terminal();
void term_clear_screen();
void move_cursor(int row, int col);
void reset_cursor();
void handle_backspace(char *buf, size_t *len);
void handle_input(char *buf, size_t *len, size_t max_len);

void free_history(void);


#endif
