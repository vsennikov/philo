/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:47:21 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/26 18:49:32 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_everything(t_data *data)
{
	int	i;

	i = 0;
	while (i != data->philos_count)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		i++;
	}
	i = 0;
	free(data->philos);
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->print_lock);
	free(data);
}

int	philo_died(t_philo *philo)
{
	if (current_time() - philo->last_eaten >= philo->data->time_to_die)
		return (pthread_mutex_lock(&philo->data->dead_lock), philo->is_dead = 1,
			pthread_mutex_unlock(&philo->data->dead_lock), 1);
	pthread_mutex_lock(&philo->data->dead_lock);
	if (philo->data->philo_dead)
		return (pthread_mutex_unlock(&philo->data->dead_lock), 1);
	pthread_mutex_unlock(&philo->data->dead_lock);
	return (0);
}

void	print_state(t_data *data, int philo_id, char *action)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%lld %d %s\n", current_time() - data->start_time, philo_id, action);
	pthread_mutex_unlock(&data->print_lock);
}

void	update_meals_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
}

void	check_during_eat(t_philo *philo)
{
	if (current_time() + philo->data->time_to_eat >= philo->last_eaten
		+ philo->data->time_to_die)
		usleep((philo->data->time_to_die - (current_time()
					- philo->data->start_time)) * 1000);
	else
		usleep(philo->data->time_to_eat * 1000);
}
