CMD_DIR = shell_commands
PAR_DIR = shell_parser
EXE_DIR = shell_executor
LIBFT_DIR = libft

SRC = main.c free.c $(CMD_DIR)/shell_cd.c $(CMD_DIR)/shell_commands.c \
	$(PAR_DIR)/parser.c $(EXE_DIR)/executor.c $(CMD_DIR)/shell_echo.c \
	shell_termcaps/termcaps.c history.c shell_env/env.c $(CMD_DIR)/shell_pwd.c \
	$(CMD_DIR)/shell_export.c $(CMD_DIR)/shell_unset.c $(CMD_DIR)/shell_env.c\

NAME = shell

CC = cc
CFLAGS = -g3 -fsanitize=address
# CFLAGS = -Wall -Werror -Wextra
LIBFT = $(LIBFT_DIR)/libft.a

all: libft $(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(SRC) $(LIBFT)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) -lreadline -ltermcap -L$(LIBFT_DIR) -lft

run: all
	./$(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f *.o

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all libft clean fclean re run
