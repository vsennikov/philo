/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:52 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/16 12:37:37 by vsenniko         ###   ########.fr       */
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

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	long long			last_eaten;
	int					meals_eaten;
	pthread_mutex_t		meal_lock;
	int					status;
	int					is_dead;
	pthread_mutex_t		dead_lock;
	int					philos_count;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					numb_of_meals;
	pthread_mutex_t		instance_lock;
	pthread_mutex_t		left_fork;
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
	int					n_of_finished;
	int					id_to_delay;
	long long			start_time;
	pthread_mutex_t		instance_lock;
	pthread_mutex_t		print_lock;
	int					finished;
	pthread_mutex_t		finished_lock;
	t_philo				*philos;
	pthread_t			monitor;
}						t_data;

// utils
int						atoi_wrap(char *str);
int						free_and_print(int flag, t_data *data);
void					ft_putstr_fd(char *s, int fd);
void					*ft_calloc(size_t nmemb, size_t size);
long long				current_time(void);
void					custom_usleep(long long sleep, t_data *data);
void					clean_everything(t_data *data);
void					clean_init(t_data *data, int t_created);

// monitors
void					*philo_loop(void *arg);
void					*monitor_loop(void *arg);

// philos utils
int						take_forks(t_philo *philo);
void					leave_forks(t_philo *philo);
int						update_meals_eaten(t_philo *philo);
int						check_during_eat(t_philo *philo);
int						philo_died(t_philo *philo);
int						print_state(t_data *data, int philo_id, char *action);
int						check_finished(t_philo *philo);
int						single_case(t_philo *philo, int id);
int						even_case(t_philo *philo, int id);
int						odd_case(t_philo *philo, int id);
void					take_right_fork(t_philo *philo);
void					leave_right_fork(t_philo *philo);
void					set_meal_num(int *numb_of_meals, t_philo *philo);
void					update_id_delay(t_philo *philo);
int						check_before_eating(t_philo *philo);
void					set_start_time(t_philo *philo);
int						print_dead(t_philo *philo);
void					calculate_and_think(t_philo *philo, int id);

// inits
int						init_philos(t_data *data);
int						init_threads(t_data *data);
#endif