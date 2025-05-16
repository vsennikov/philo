/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_job.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:19:08 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/16 11:53:21 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	somebody_died(t_data *data, int i);
static int	work_with_meals(t_data *data, int i);

void	*monitor_loop(void *arg)
{
	t_data		*data;
	int			i;
	static int	p_count;

	data = arg;
	p_count = data->philos_count;
	while (1)
	{
		i = 0;
		while (i != p_count)
		{
			if (data->numb_of_meals != -2)
			{
				if (work_with_meals(data, i))
					return (NULL);
			}
			if (somebody_died(data, i))
				return (NULL);
			i++;
		}
		custom_usleep(100);
	}
	return (NULL);
}

static int	work_with_meals(t_data *data, int i)
{
	pthread_mutex_lock(&data->philos[i].meal_lock);
	if (data->philos[i].meals_eaten == data->numb_of_meals
		&& !data->philos[i].status)
	{
		data->philos[i].status = 1;
		pthread_mutex_lock(&data->finished_lock);
		data->n_of_finished++;
		if (data->n_of_finished == data->philos_count)
			return (data->finished = 1,
				pthread_mutex_unlock(&data->finished_lock),
				pthread_mutex_unlock(&data->philos[i].meal_lock), 1);
		pthread_mutex_unlock(&data->finished_lock);
	}
	pthread_mutex_unlock(&data->philos[i].meal_lock);
	return (0);
}

static int	somebody_died(t_data *data, int i)
{
	pthread_mutex_lock(&data->philos[i].meal_lock);
	if (data->philos[i].numb_of_meals != -2 && data->philos[i].status)
		return (pthread_mutex_unlock(&data->philos[i].meal_lock), 0);
	pthread_mutex_unlock(&data->philos[i].meal_lock);
	if (philo_died(&data->philos[i]))
		return (1);
	return (0);
}
