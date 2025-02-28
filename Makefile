SRC_DIR    = src
CMD_DIR    = buildin_commands
LIBFT_DIR  = libft

SRC = $(SRC_DIR)/$(CMD_DIR)/shell_cd.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_echo.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_env.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_export.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_pwd.c \
      $(SRC_DIR)/$(CMD_DIR)/shell_unset.c \
      $(SRC_DIR)/main.c \
      $(SRC_DIR)/env.c \
      $(SRC_DIR)/executor.c \
      $(SRC_DIR)/parser.c \
      $(SRC_DIR)/termcaps.c \
      $(SRC_DIR)/singal.c \
      $(SRC_DIR)/terminal.c \
      $(SRC_DIR)/free.c \

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
	rm -f $(OBJ)

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
