/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:49 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/12 17:22:08 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_data(t_data *data, int argc, char **argv)
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

static int	init_everything(t_data *data, int argc, char **argv)
{
	if (!init_data(data, argc, argv))
		return (free(data), 0);
	if (!init_philos(data))
		return (pthread_mutex_destroy(&data->print_lock),
			pthread_mutex_destroy(&data->finished_lock),
			pthread_mutex_destroy(&data->instance_lock), free(data), 0);
	if (init_threads(data))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	if (argc < 5 || argc > 6)
		return (ft_putstr_fd(NO_ARGS, 2), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (free_and_print(DATA_ERR, data));
	if (!init_everything(data, argc, argv))
		return (1);
	i = 0;
	while (i < data->philos_count)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
	clean_everything(data);
	return (0);
}
