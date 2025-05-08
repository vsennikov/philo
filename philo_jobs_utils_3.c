/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs_utils_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:14:43 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/08 14:28:52 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	single_case(t_philo *philo, int id)
{
	pthread_mutex_unlock(&philo->instance_lock);
	pthread_mutex_lock(&philo->left_fork);
	print_state(philo->data, id + 1, "has taken a fork");
	custom_usleep(philo->data->time_to_die * 1000);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_lock(&philo->dead_lock);
	philo->is_dead = 1;
	return (pthread_mutex_unlock(&philo->dead_lock), 0);
}

int	even_case(t_philo *philo, int id)
{
	pthread_mutex_unlock(&philo->instance_lock);
	if (check_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->left_fork);
	print_state(philo->data, id + 1, "has taken a fork");
	if (check_finished(philo))
		return (pthread_mutex_unlock(&philo->left_fork), 0);
	take_right_fork(philo);
	print_state(philo->data, id + 1, "has taken a fork");
	return (1);
}

int	odd_case(t_philo *philo, int id)
{
	pthread_mutex_unlock(&philo->instance_lock);
	if (check_finished(philo))
		return (0);
	take_right_fork(philo);
	print_state(philo->data, id + 1, "has taken a fork");
	if (check_finished(philo))
		return (leave_right_fork(philo), 0);
	pthread_mutex_lock(&philo->left_fork);
	print_state(philo->data, id + 1, "has taken a fork");
	return (1);
}

void	set_meal_num(int *numb_of_meals, t_philo *philo)
{
	if (!(*numb_of_meals))
	{
		pthread_mutex_lock(&philo->meal_lock);
		*numb_of_meals = philo->numb_of_meals;
		pthread_mutex_unlock(&philo->meal_lock);
	}
}

int	check_before_eating(t_philo *philo)
{
	if (philo->last_eaten && current_time() - philo->time_to_die == philo->last_eaten)
	{
		pthread_mutex_lock(&philo->dead_lock);
		philo->is_dead = 1;
		pthread_mutex_unlock(&philo->dead_lock);
		pthread_mutex_unlock(&philo->instance_lock);
		leave_forks(philo);
		return (0);
	}
	return (1);
}
