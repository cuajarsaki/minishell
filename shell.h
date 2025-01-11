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

typedef struct {
	char **tokens;
	size_t count;
} LexerBuffer;

void ignore_signals(void);
void set_prompt(const char *prompt);
void process_command(char *input);
void shell_clear(void);
void shell_cd(char *path);
void shell_pwd(void);
void shell_ls(char **args);
void shell_unknown_command(char *cmd);
void lexer_build(char *input, LexerBuffer *lexerbuf);
void lexer_free(LexerBuffer *lexerbuf);
ASTreeNode *parse_tokens(LexerBuffer *lexerbuf);
void	execute_syntax_tree(ASTreeNode *node);
void	shell_echo(char **args);
void	shell_man(char *arg);


//TERMCAMPS
void reset_terminal_settings();
void setup_terminal();
void term_clear_screen();
void move_cursor(int row, int col);
void reset_cursor();
void handle_backspace(char *buf, size_t *len);
void handle_input(char *buf, size_t *len, size_t max_len);

#endif
