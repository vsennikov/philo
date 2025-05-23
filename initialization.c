/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:21:01 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/16 11:54:26 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	clean_for_init_philos(t_data *data, int init_one, int flag,
				int flag_for_clean);

int	init_philo_mutexs(t_data *data, int i)
{
	if (pthread_mutex_init(&data->philos[i].left_fork, NULL))
		return (clean_for_init_philos(data, i, L_FORK_ERR, -1));
	if (pthread_mutex_init(&data->philos[i].meal_lock, NULL))
		return (clean_for_init_philos(data, i, MEAL_ERR, 0));
	if (pthread_mutex_init(&data->philos[i].dead_lock, NULL))
		return (clean_for_init_philos(data, i, DEAD_MUT_ERR, 1));
	if (pthread_mutex_init(&data->philos[i].instance_lock, NULL))
		return (clean_for_init_philos(data, i, I_LOCK, 2));
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
			return (data->finished = 1,
				pthread_mutex_unlock(&data->instance_lock), clean_init(data, i),
				1);
		i++;
	}
	data->start_time = current_time();
	if (pthread_create(&data->monitor, NULL, monitor_loop, data))
		return (pthread_mutex_unlock(&data->instance_lock),
			pthread_mutex_lock(&data->finished_lock), data->finished = 1,
			pthread_mutex_unlock(&data->finished_lock), clean_init(data, i), 1);
	pthread_mutex_unlock(&data->instance_lock);
	return (0);
}

void	clean_mutex_for_philo(int flag, t_data *data, int i)
{
	if (flag == 0)
		pthread_mutex_destroy(&data->philos[i].left_fork);
	else if (flag == 1)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
	}
	else if (flag == 2)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		pthread_mutex_destroy(&data->philos[i].dead_lock);
	}
	else if (flag == 3)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		pthread_mutex_destroy(&data->philos[i].dead_lock);
		pthread_mutex_destroy(&data->philos[i].instance_lock);
	}
}

static int	clean_for_init_philos(t_data *data, int init_one, int flag,
		int flag_for_clean)
{
	int	i;

	i = 0;
	if (init_one == 1)
	{
		clean_mutex_for_philo(flag_for_clean, data, i);
		return (free_and_print(flag, data));
	}
	while (i != init_one)
	{
		if (i != init_one - 1)
			clean_mutex_for_philo(3, data, i);
		else
			clean_mutex_for_philo(flag_for_clean, data, i);
		i++;
	}
	free(data->philos);
	return (free_and_print(flag, data));
}
