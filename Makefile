CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c philo_utils.c ft_atoi.c philo_jobs.c init.c
OBJ = $(SRCS:.c=.o)

NAME = philo

all: $(NAME) 

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME) 

re: fclean all

.PHONY: all clean fclean re