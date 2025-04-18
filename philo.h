/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:52 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/18 17:34:22 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "errmsg.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define START 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	long long			last_eaten;
	int					meals_eaten;
	int					status;
	int					is_dead;
	pthread_mutex_t		left_fork;
	pthread_mutex_t		meal_lock;
	// pthread_mutex_t	right_fork;
	pthread_t			thread;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					philos_count;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					numb_of_meals;
	int					philo_dead;
	long long			start_time;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		print_lock;
	t_philo				*philos;
	pthread_t			monitor;
}						t_data;

// utils
int						atoi_wrap(char *str);
int						ft_strlen(char *str);
void					ft_putstr_fd(char *s, int fd);
long long				current_time(void);
void					clean_everything(t_data *data);
void					*ft_calloc(size_t nmemb, size_t size);
// philos funcs
void					*philo_loop(void *arg);
void					print_state(t_data *data, int philo_id, char *action);
// inits
int						init_data_str(t_data *data, int argc, char **argv);

#endif