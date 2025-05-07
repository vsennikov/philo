/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:21:01 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/07 15:55:44 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	clean_for_init_philos(t_data *data, int init_one, int flag);

int	init_data(t_data *data, int argc, char **argv)
{
	data->philos_count = atoi_wrap(argv[1]);
	data->time_to_die = atoi_wrap(argv[2]);
	data->time_to_eat = atoi_wrap(argv[3]);
	data->time_to_sleep = atoi_wrap(argv[4]);
	data->numb_of_meals = -2;
	if (argc == 6)
		data->numb_of_meals = atoi_wrap(argv[5]);
	if (data->philos_count == -1 || data->time_to_die == -1
		|| data->time_to_eat == -1 || data->time_to_sleep == -1
		|| data->numb_of_meals == -1)
		return (free_and_print(NOT_VALID_ERR, data));
	data->philo_dead = 0;
	data->philos = ft_calloc(data->philos_count, sizeof(t_philo));
	if (!data->philos)
		return (free_and_print(CALLOC_ERR, data));
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (free_and_print(PRINT_MUT_ERR, data));
	if (pthread_mutex_init(&data->finished_lock, NULL))
		return (free_and_print(FINISH_MUT_ERR, data));
	if (pthread_mutex_init(&data->instance_lock, NULL))
		return (free_and_print(FINISH_MUT_ERR, data));
	data->id_to_delay = 2;
	return (data->n_of_finished = 0, data->finished = 0, 1);
}

int	init_philo_mutexs(t_data *data, int i)
{
	if (pthread_mutex_init(&data->philos[i].left_fork, NULL))
		return (clean_for_init_philos(data, i + 1, L_FORK_ERR));
	if (pthread_mutex_init(&data->philos[i].meal_lock, NULL))
		return (pthread_mutex_destroy(&data->philos[i].left_fork),
			clean_for_init_philos(data, i + 1, MEAL_ERR));
	if (pthread_mutex_init(&data->philos[i].dead_lock, NULL))
		return (pthread_mutex_destroy(&data->philos[i].left_fork),
			pthread_mutex_destroy(&data->philos[i].meal_lock),
			clean_for_init_philos(data, i + 1, DEAD_MUT_ERR));
	if (pthread_mutex_init(&data->philos[i].instance_lock, NULL))
		return (pthread_mutex_destroy(&data->philos[i].left_fork),
			pthread_mutex_destroy(&data->philos[i].meal_lock),
			pthread_mutex_destroy(&data->philos[i].dead_lock),
			clean_for_init_philos(data, i + 1, DEAD_MUT_ERR));
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i != data->philos_count)
	{
		data->philos[i].id = i;
		data->philos[i].last_eaten = 0;
		data->philos[i].meals_eaten = 0;
		data->philos[i].status = START;
		data->philos[i].is_dead = 0;
		data->philos[i].time_to_die = data->time_to_die;
		data->philos[i].time_to_eat = data->time_to_eat;
		data->philos[i].time_to_sleep = data->time_to_sleep;
		data->philos[i].philos_count = data->philos_count;
		data->philos[i].numb_of_meals = data->numb_of_meals;
		if (!init_philo_mutexs(data, i))
			return (0);
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

int	init_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->instance_lock);
	while (i < data->philos_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_loop,
				&data->philos[i]))
			return (clean_init(data, i), 1);
		i++;
	}
	data->start_time = current_time();
	pthread_mutex_unlock(&data->instance_lock);
	if (pthread_create(&data->monitor, NULL, monitor_loop, data))
		return (clean_init(data, i + 1), 1);
	return (0);
}

static int	clean_for_init_philos(t_data *data, int init_one, int flag)
{
	int	i;

	i = 0;
	while (i != init_one)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		i++;
	}
	free(data->philos);
	return (free_and_print(flag, data));
}
