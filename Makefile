NAME = philo
SRC = 1_main.c 2_init.c 3_philo.c 4_actions.c 5_check_dead_full.c utils.c 
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