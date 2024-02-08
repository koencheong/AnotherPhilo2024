NAME = philo
SRC = main.c utils.c init.c philo.c actions.c check_dead_full.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread
# CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all