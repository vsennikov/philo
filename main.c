/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:49 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/26 21:00:25 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	work_with_meals(t_data *data, int i)
{
	pthread_mutex_lock(&data->philos[i].meal_lock);
	if (data->philos[i].meals_eaten == data->numb_of_meals
		&& !data->philos[i].status)
	{
		data->philos[i].status = 1;
		pthread_mutex_lock(&data->dead_lock);
		data->n_of_finished++;
		if (data->n_of_finished == data->philos_count)
			return (pthread_mutex_unlock(&data->dead_lock), 1);
		pthread_mutex_unlock(&data->dead_lock);
	}
	pthread_mutex_unlock(&data->philos[i].meal_lock);
	return (0);
}

static void	*monitor_loop(void *arg)
{
	t_data	*data;
	int		i;

	data = arg;
	while (1)
	{
		i = 0;
		while (i != data->philos_count)
		{
			if (data->numb_of_meals != -2)
			{
				if (work_with_meals(data, i))
					return (NULL);
			}
			pthread_mutex_lock(&data->dead_lock);
			if (data->philos[i].is_dead)
				return (pthread_mutex_unlock(&data->dead_lock),
					data->philo_dead = 1, print_state(data, i + 1, "died"),
					NULL);
			pthread_mutex_unlock(&data->dead_lock);
			i++;
		}
	}
	return (NULL);
}

void	init_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = current_time();
	while (i < data->philos_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_loop,
				&data->philos[i]))
			clean_everything(data);
		i += 2;
	}
	usleep(1);
	i = 1;
	while (i < data->philos_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_loop,
				&data->philos[i]))
			clean_everything(data);
		i += 2;
	}
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (ft_putstr_fd(NO_ARGS, 2), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_putstr_fd(ALLOC_GENERAL, 2), 1);
	if (!init_data_str(data, argc, argv))
		return (free(data), 1);
	init_threads(data);
	if (pthread_create(&data->monitor, NULL, monitor_loop, data))
		clean_everything(data);
	pthread_join(data->monitor, NULL);
	clean_everything(data);
	return (0);
}
