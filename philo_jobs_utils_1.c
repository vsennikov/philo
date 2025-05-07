/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs_utils_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:55:21 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/07 12:39:11 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	int	id;

	if (check_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->instance_lock);
	id = philo->id;
	if (philo->philos_count == 1)
		return (single_case(philo, id));
	else if ((id + 1) % 2 == 0)
	{
		if (!even_case(philo, id))
			return (0);
	}
	else
	{
		if (!odd_case(philo, id))
			return (0);
	}
	return (1);
}

void	leave_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->instance_lock);
	if ((philo->id + 1) % 2 == 0)
	{
		pthread_mutex_unlock(&philo->left_fork);
		leave_right_fork(philo);
	}
	else
	{
		leave_right_fork(philo);
		pthread_mutex_unlock(&philo->left_fork);
	}
	pthread_mutex_unlock(&philo->instance_lock);
}

int	update_meals_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_eaten++;
	if (philo->meals_eaten == philo->numb_of_meals)
		return (pthread_mutex_unlock(&philo->meal_lock), 1);
	pthread_mutex_unlock(&philo->meal_lock);
	return (0);
}

void	take_right_fork(t_philo *philo)
{
	if (philo->philos_count != 1 && philo->id == philo->philos_count - 1
		&& philo->id != 0)
		pthread_mutex_lock(&philo->data->philos[0].left_fork);
	else if (philo->philos_count != 1)
		pthread_mutex_lock(&philo->data->philos[philo->id + 1].left_fork);
}

void	leave_right_fork(t_philo *philo)
{
	if (philo->philos_count != 1 && philo->id == philo->philos_count - 1
		&& philo->id != 0)
		pthread_mutex_unlock(&philo->data->philos[0].left_fork);
	else
		pthread_mutex_unlock(&philo->data->philos[philo->id + 1].left_fork);
}
