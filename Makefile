CC = cc
CFLAGS = -Wall -Wextra -Werror -g
# CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread

SRCS = main.c philo_utils_1.c philo_utils_2.c error.c initialization.c monitor_job.c\
 philo_jobs.c philo_jobs_utils_1.c philo_jobs_utils_2.c philo_jobs_utils_3.c philo_jobs_utils_4.c

OBJ = $(SRCS:.c=.o)

NAME = philo

all: $(NAME) 

$(NAME): $(OBJ) 
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philo.h Makefile
	@$(CC) $(CFLAGS)  -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME) 

re: fclean all

.PHONY: all clean fclean re