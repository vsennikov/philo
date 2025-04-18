/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:49 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/18 18:32:59 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_loop(void *arg)
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
				pthread_mutex_lock(&data->philos[i].meal_lock);
				if (data->philos[i].meals_eaten == data->numb_of_meals)
					return (pthread_mutex_unlock(&data->philos[i].meal_lock),
						data->philo_dead = 3, NULL);
				pthread_mutex_unlock(&data->philos[i].meal_lock);
			}
			pthread_mutex_lock(&data->dead_lock);
			if (data->philos[i].is_dead)
				return (print_state(data, i + 1, "died"),
					pthread_mutex_unlock(&data->dead_lock),
					data->philo_dead = 1, NULL);
			pthread_mutex_unlock(&data->dead_lock);
			i++;
		}
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	if (argc < 5 || argc > 6)
		return (ft_putstr_fd(NO_ARGS, 2), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_putstr_fd(ALLOC_GENERAL, 2), 1);
	if (!init_data_str(data, argc, argv))
		return (free(data), 1);
	i = 0;
	data->start_time = current_time();
	while (i != data->philos_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_loop,
				&data->philos[i]))
			clean_everything(data);
		usleep(1);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_loop, data))
		clean_everything(data);
	pthread_join(data->monitor, NULL);
	// clean_everything(data);
	return (100);
}
