/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:57:44 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/09 12:07:51 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_during_eat(t_philo *philo)
{
	if (check_finished(philo))
	{
		leave_forks(philo);
		return (0);
	}
	custom_usleep(philo->time_to_eat * 1000);
	return (1);
}

int	philo_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->dead_lock);
	pthread_mutex_lock(&philo->instance_lock);
	if (check_finished(philo))
		return (pthread_mutex_unlock(&philo->dead_lock),
			pthread_mutex_unlock(&philo->instance_lock), 1);
	if (philo->last_eaten && current_time()
		- philo->last_eaten >= philo->time_to_die)
	{
		pthread_mutex_unlock(&philo->instance_lock);
		pthread_mutex_lock(&philo->data->print_lock);
		pthread_mutex_lock(&philo->data->finished_lock);
		pthread_mutex_lock(&philo->data->instance_lock);
		philo->is_dead = 1;
		printf("%lld %d %s\n", current_time() - philo->data->start_time,
			philo->id + 1, "died");
		philo->data->finished = 1;
		pthread_mutex_unlock(&philo->data->instance_lock);
		pthread_mutex_unlock(&philo->data->finished_lock);
		pthread_mutex_unlock(&philo->dead_lock);
		pthread_mutex_unlock(&philo->data->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->dead_lock);
	pthread_mutex_unlock(&philo->instance_lock);
	return (0);
}

int	print_state(t_data *data, int philo_id, char *action)
{
	pthread_mutex_lock(&data->print_lock);
	pthread_mutex_lock(&data->finished_lock);
	if (data->finished)
	{
		pthread_mutex_unlock(&data->finished_lock);
		pthread_mutex_unlock(&data->print_lock);
		return (0);
	}
	pthread_mutex_unlock(&data->finished_lock);
	pthread_mutex_lock(&data->instance_lock);
	printf("%lld %d %s\n", current_time() - data->start_time, philo_id, action);
	pthread_mutex_unlock(&data->instance_lock);
	pthread_mutex_unlock(&data->print_lock);
	return (1);
}

void	update_id_delay(t_philo *philo)
{
	if (philo->data->id_to_delay >= philo->data->philos_count - 1
		&& philo->data->id_to_delay == philo->id)
		philo->data->id_to_delay = 0;
	else if (philo->data->id_to_delay == philo->id
		&& philo->data->philos_count == 3)
		philo->data->id_to_delay += 1;
	else if (philo->data->id_to_delay == philo->id)
		philo->data->id_to_delay += 2;
}

void	set_start_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->instance_lock);
	pthread_mutex_lock(&philo->data->instance_lock);
	philo->last_eaten = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->instance_lock);
	pthread_mutex_unlock(&philo->instance_lock);
}
