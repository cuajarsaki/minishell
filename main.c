#include "shell.h"





/*********
 * DEBUG *
 *********/

#define COLOR_BLUE     "\033[34m"  // Command Table
#define COLOR_GREEN    "\033[32m"  // Command
#define COLOR_YELLOW   "\033[33m"  // Token
#define COLOR_CYAN     "\033[36m"  // Redirection
#define COLOR_MAGENTA  "\033[35m"  // seperator
#define COLOR_RESET    "\033[0m"   // Reset to default

/* Print indentation based on depth */
void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("\t");
}

/* Debug function to print tokens */
void print_tokens(t_list *tokens, int depth)
{
    while (tokens)
    {
        print_indent(depth);
        printf(COLOR_YELLOW "|    - Token: %s\n" COLOR_RESET, (char *)tokens->content);
        tokens = tokens->next;
    }
}

/* Debug function to print redirections */
void print_redirs(t_list *redirs, int depth)
{
    while (redirs)
    {
        t_redir *redir = (t_redir *)redirs->content;
        print_indent(depth);
        printf(COLOR_CYAN "|    - Redir: %s %s\n" COLOR_RESET, redir->type, redir->direction);
        redirs = redirs->next;
    }
}

/* Debug function to print commands */
void print_cmds(t_list *cmds, int depth)
{
    while (cmds)
    {
        t_cmd *cmd = (t_cmd *)cmds->content;
        
        print_indent(depth);
        printf(COLOR_GREEN "|  +----------------------------+\n" COLOR_RESET);

        print_indent(depth);
        printf(COLOR_GREEN "|  | Command: %s\n" COLOR_RESET, (char *)cmd->tokens->content);
        
        print_tokens(cmd->tokens->next, depth + 1); // Skip first token (command name)
        print_redirs(cmd->redirs, depth + 1);
        
        print_indent(depth);
        printf(COLOR_GREEN "|  +----------------------------+\n" COLOR_RESET);
        
        cmds = cmds->next;
    }
}

/* Debug function to print command tables */
void print_command_groups(t_list *command_groups, int depth)
{
    while (command_groups)
    {
        t_command_group *command_group = (t_command_group *)command_groups->content;
        
        print_indent(depth);
        printf(COLOR_BLUE "|  +--------------------------------+\n" COLOR_RESET);
        
        print_indent(depth);
        printf(COLOR_BLUE "|  | Command Table                  |\n" COLOR_RESET);
        
        print_cmds(command_group->cmds, depth + 1);
        
        print_indent(depth);
        printf(COLOR_MAGENTA "|  | seperator: \"%s\"\n" COLOR_RESET, command_group->seperator);
        
        print_indent(depth);
        printf(COLOR_BLUE "|  +--------------------------------+\n" COLOR_RESET);
        
        command_groups = command_groups->next;
    }
}

/* Main debug function to print AST */
void debug_ast(t_ast *ast)
{
    if (!ast)
    {
        printf("AST is NULL\n");
        return;
    }

    printf("+====================================+\n");
    printf("| " COLOR_BLUE "AST\n" COLOR_RESET);
    print_command_groups(ast->command_groups, 1);
	printf("+====================================+\n");
}

/***************
 * ACTUAL MAIN *
 ***************/




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

	t_ast	*current_AST;

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
			current_AST = get_ast(buf, env_list);
			debug_ast(current_AST);
			exec_ast(current_AST, env_list);
            free_ast(current_AST);
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