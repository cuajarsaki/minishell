CMD_DIR    = shell_commands
PAR_DIR    = shell_parser
EXE_DIR    = shell_executor
LIBFT_DIR  = libft

SRC = main.c free.c singal.c terminal.c \
      $(CMD_DIR)/shell_cd.c $(CMD_DIR)/shell_commands.c \
      $(PAR_DIR)/parser.c $(EXE_DIR)/executor.c $(CMD_DIR)/shell_echo.c \
      shell_termcaps/termcaps.c history.c shell_env/env.c \
      $(CMD_DIR)/shell_pwd.c $(CMD_DIR)/shell_export.c \
      $(CMD_DIR)/shell_unset.c $(CMD_DIR)/shell_env.c custom_implements.c

OBJ = $(SRC:.c=.o)

NAME    = minishell
HEADERS = minishell.h

CC      = cc
# CFLAGS = -Wall -Werror -Wextra
CFLAGS  = -g3 -fsanitize=address 

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
