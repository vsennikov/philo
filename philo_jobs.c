/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:28:42 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/26 20:38:16 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_philo *philo)
{
	if (philo->data->philos_count == 1)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_state(philo->data, philo->id + 1, "has taken a fork");
		usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(&philo->left_fork);
		philo->is_dead = 1;
		return (0);
	}
	else if ((philo->id + 1) % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_state(philo->data, philo->id + 1, "has taken a fork");
		take_right_fork(philo);
		print_state(philo->data, philo->id + 1, "has taken a fork");
	}
	else
	{
		take_right_fork(philo);
		print_state(philo->data, philo->id + 1, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_state(philo->data, philo->id + 1, "has taken a fork");
	}
	return (1);
}

static int	p_eat(t_philo *philo)
{
	if (philo_died(philo))
		return (0);
	if (!take_forks(philo))
		return (0);
	print_state(philo->data, philo->id + 1, "is eating");
	check_during_eat(philo);
	if (philo_died(philo))
		return (0);
	leave_right_fork(philo);
	pthread_mutex_unlock(&philo->data->philos[philo->id].left_fork);
	philo->last_eaten = current_time();
	if (philo->data->numb_of_meals != -2)
		update_meals_eaten(philo);
	return (1);
}

static int	p_sleep(t_philo *philo)
{
	if (philo_died(philo))
		return (0);
	print_state(philo->data, philo->id + 1, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	return (1);
}

static int	p_think(t_philo *philo)
{
	if (philo_died(philo))
		return (0);
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
		pthread_mutex_lock(&philo->meal_lock);
		if (philo->data->numb_of_meals != -2
			&& philo->data->numb_of_meals == philo->meals_eaten)
			return (pthread_mutex_unlock(&philo->meal_lock), NULL);
		pthread_mutex_unlock(&philo->meal_lock);
		if (!p_sleep(philo))
			return (NULL);
		if (!p_think(philo))
			return (NULL);
	}
	return (NULL);
}
