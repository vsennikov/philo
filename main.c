/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:00:49 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/12 17:19:47 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
