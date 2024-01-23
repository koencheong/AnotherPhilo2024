NAME = philo
SRC = main.c utils.c init.c safe_functions.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all