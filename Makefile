SRC_DIR    = src
CMD_DIR    = buildin_commands
LIBFT_DIR  = libft

SRC = $(SRC_DIR)/$(CMD_DIR)/shell_cd.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_echo.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_env.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_export.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_pwd.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_unset.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_exit.c \
      $(SRC_DIR)/main.c \
      $(SRC_DIR)/env/env.c \
	  $(SRC_DIR)/env/env_2.c \
	  $(SRC_DIR)/env/env_3.c \
      $(SRC_DIR)/termcaps/termcaps.c \
      $(SRC_DIR)/signal/signal.c \
	  $(SRC_DIR)/signal/signal_2.c \
      $(SRC_DIR)/terminal/terminal.c \
      $(SRC_DIR)/free/free.c \
	  $(SRC_DIR)/free/free_2.c \
	  $(SRC_DIR)/free/free_3.c \
      $(SRC_DIR)/cmd_tokenizer/cmd_tokenizer.c \
	  $(SRC_DIR)/parser/parser_ast.c \
	  $(SRC_DIR)/parser/parser_cmd.c \
	  $(SRC_DIR)/parser/parser_env.c \
	  $(SRC_DIR)/parser/parser_redir.c \
	  $(SRC_DIR)/parser/parser_token_utils.c \
	  $(SRC_DIR)/parser/parser_token.c \
	  $(SRC_DIR)/parser/parser_utils.c \
	  $(SRC_DIR)/executor/executor_ast.c \
	  $(SRC_DIR)/executor/executor_builtin_check.c \
	  $(SRC_DIR)/executor/executor_command_group.c \
	  $(SRC_DIR)/executor/executor_command_execution1.c \
	  $(SRC_DIR)/executor/executor_command_execution2.c \
	  $(SRC_DIR)/executor/executor_child.c \
	  $(SRC_DIR)/executor/executor_command_path.c \
	  $(SRC_DIR)/executor/executor_command_pipe.c \
	  $(SRC_DIR)/executor/executor_command_buildin.c \
	  $(SRC_DIR)/executor/executor_file_desc.c \
	  $(SRC_DIR)/executor/executor_parent.c \
	  $(SRC_DIR)/executor/executor_path.c \
	  $(SRC_DIR)/executor/executor_utility.c \
	  $(SRC_DIR)/executor/executor_redirections.c \

OBJ = $(SRC:.c=.o)

NAME    = minishell
HEADERS = minishell.h

CC      = cc
CFLAGS  = -Wall -Werror -Wextra -g3 -fsanitize=address 

LIBFT   = $(LIBFT_DIR)/libft.a

all: libft $(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline -ltermcap -L$(LIBFT_DIR) -lft	

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all libft clean fclean re
