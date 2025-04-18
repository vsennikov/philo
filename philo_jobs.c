/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:28:42 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/18 18:11:19 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_data *data, int philo_id, char *action)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%lld %d %s\n", current_time() - data->start_time, philo_id, action);
	pthread_mutex_unlock(&data->print_lock);
}

static int	p_eat(t_philo *philo)
{
	if (current_time() - philo->last_eaten >= philo->data->time_to_die)
		return (pthread_mutex_lock(&philo->data->dead_lock), philo->is_dead = 1,
			pthread_mutex_unlock(&philo->data->dead_lock), 0);
	pthread_mutex_lock(&philo->left_fork);
	print_state(philo->data, philo->id + 1, "has taken a fork");
	if (philo->data->philos_count == 1)
		return (usleep(philo->data->time_to_die * 1000),
			pthread_mutex_unlock(&philo->left_fork), philo->is_dead = 1, 0);
	if (philo->data->philos_count != 1 && philo->id == philo->data->philos_count
		- 1)
		pthread_mutex_lock(&philo->data->philos[0].left_fork);
	else if (philo->data->philos_count != 1)
		pthread_mutex_lock(&philo->data->philos[philo->id + 1].left_fork);
	print_state(philo->data, philo->id + 1, "has taken a fork");
	print_state(philo->data, philo->id + 1, "is eating");
	usleep(philo->data->time_to_eat * 1000);
	if (philo->data->philos_count != 1 && philo->id == philo->data->philos_count
		- 1 && philo->id != 0)
		pthread_mutex_unlock(&philo->data->philos[0].left_fork);
	else if (philo->data->philos_count != 1)
		pthread_mutex_unlock(&philo->data->philos[philo->id + 1].left_fork);
	pthread_mutex_unlock(&philo->data->philos[philo->id].left_fork);
	if (current_time() - philo->last_eaten >= philo->data->time_to_die)
		return (pthread_mutex_lock(&philo->data->dead_lock), philo->is_dead = 1,
			pthread_mutex_unlock(&philo->data->dead_lock), 0);
	philo->last_eaten = current_time();
	if (philo->data->numb_of_meals != -2)
	{
		pthread_mutex_lock(&philo->meal_lock);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->meal_lock);
	}
	return (1);
}

static int	p_sleep(t_philo *philo)
{
	if (current_time() - philo->last_eaten >= philo->data->time_to_die)
		return (pthread_mutex_lock(&philo->data->dead_lock), philo->is_dead = 1,
			pthread_mutex_unlock(&philo->data->dead_lock), 0);
	print_state(philo->data, philo->id + 1, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	return (1);
}

static int	p_think(t_philo *philo)
{
	if (current_time() - philo->last_eaten >= philo->data->time_to_die)
		return (pthread_mutex_lock(&philo->data->dead_lock), philo->is_dead = 1,
			pthread_mutex_unlock(&philo->data->dead_lock), 0);
	print_state(philo->data, philo->id + 1, "is thinking");
	usleep(100);
	return (1);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		if (!p_eat(philo))
			return (NULL);
		if (!p_sleep(philo))
			return (NULL);
		if (!p_think(philo))
			return (NULL);
	}
	return (NULL);
}
