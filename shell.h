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

typedef struct ASTreeNode {
	char *command;
	char **args;
	struct ASTreeNode *left;
	struct ASTreeNode *right;
} ASTreeNode;

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;


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

// Executor Functions
void execute_syntax_tree(ASTreeNode *node, t_env *env_list);




// History management functions
void add_to_history(char *command);
const char *get_history(int direction);

void ignore_signals(void);
void set_prompt(const char *prompt, t_env *env_list);
void process_command(char *input, t_env *env_list);
void shell_clear(void);
void shell_cd(char *path);
void shell_pwd(void);
void shell_ls(char **args);
void shell_unknown_command(char *cmd);
void lexer_build(char *input, LexerBuffer *lexerbuf);
void lexer_free(LexerBuffer *lexerbuf);
ASTreeNode *parse_tokens(LexerBuffer *lexerbuf);
void	shell_echo(char **args);
void	shell_man(char *arg);

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
