/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:03:45 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/22 17:32:09 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_init(t_data *data, int init_one)
{
	int	i;

	i = 0;
	while (i != init_one)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		free(&data->philos[i]);
		i++;
	}
	free(data->philos);
}

static int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i != data->philos_count)
	{
		data->philos[i].id = i;
		data->philos[i].last_eaten = current_time();
		data->philos[i].meals_eaten = 0;
		data->philos[i].status = START;
		data->philos[i].is_dead = 0;
		if (pthread_mutex_init(&data->philos[i].left_fork, NULL))
			return (clean_init(data, i + 1), 0);
		if (pthread_mutex_init(&data->philos[i].meal_lock, NULL))
			return (pthread_mutex_destroy(&data->philos[i].left_fork),
				clean_init(data, i + 1), 0);
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

int	init_data_str(t_data *data, int argc, char **argv)
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
		return (ft_putstr_fd(NON_VALID_INPUT, 2), 0);
	data->philo_dead = 0;
	data->philos = ft_calloc(data->philos_count, sizeof(t_philo));
	if (!data->philos)
		return (ft_putstr_fd(ALLOC_PHILOS, 2), 0);
	if (pthread_mutex_init(&data->dead_lock, NULL))
		return (free(data->philos), ft_putstr_fd(DEAD_MUTEXS, 2), 0);
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (free(data->philos), pthread_mutex_destroy(&data->dead_lock),
			ft_putstr_fd(PRINTING_MUTEXS, 2), 0);
	if (!init_philos(data))
		return (pthread_mutex_destroy(&data->dead_lock),
			pthread_mutex_destroy(&data->print_lock), ft_putstr_fd(ALLOC_PHILOS,
				2), 0);
	return (data->n_of_finished = 0, 1);
}
