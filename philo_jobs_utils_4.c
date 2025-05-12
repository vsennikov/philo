/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs_utils_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:20:11 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/12 14:13:38 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_dead(t_philo *philo)
{
	pthread_mutex_unlock(&philo->instance_lock);
	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->instance_lock);
	philo->is_dead = 1;
	if (check_finished(philo))
		return (pthread_mutex_unlock(&philo->data->print_lock),
			pthread_mutex_unlock(&philo->data->instance_lock),
			pthread_mutex_unlock(&philo->dead_lock), 0);
	pthread_mutex_lock(&philo->data->finished_lock);
	if (!philo->data->finished)
		printf("%lld %d %s\n", current_time() - philo->data->start_time,
			philo->id + 1, "died");
	philo->data->finished = 1;
	pthread_mutex_unlock(&philo->data->instance_lock);
	pthread_mutex_unlock(&philo->data->finished_lock);
	pthread_mutex_unlock(&philo->dead_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
	return (1);
}

void	calculate_and_think(t_philo *philo, int id)
{
	int	delay;

	delay = 0;
	if (philo->id == id && philo->philos_count % 2 != 0)
	{
		if (philo->time_to_eat > philo->time_to_sleep)
			delay = (philo->time_to_eat - philo->time_to_sleep) * 1000;
		custom_usleep(delay + 10000);
	}
	else if (philo->philos_count % 2 != 0)
	{
		if (philo->time_to_eat > philo->time_to_sleep)
			delay = (philo->time_to_eat - philo->time_to_sleep) * 1000;
		custom_usleep(delay + 1000);
	}
}
