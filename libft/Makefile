CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC = ft_strtok.c ft_strchr.c

NAME = libft.a

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	ar rcs $(NAME) *.o

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
