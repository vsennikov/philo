/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:41:57 by vsenniko          #+#    #+#             */
/*   Updated: 2025/04/26 18:42:08 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_right_fork(t_philo *philo)
{
	if (philo->data->philos_count != 1 && philo->id == philo->data->philos_count
		- 1)
		pthread_mutex_lock(&philo->data->philos[0].left_fork);
	else if (philo->data->philos_count != 1)
		pthread_mutex_lock(&philo->data->philos[philo->id + 1].left_fork);
}

void	leave_right_fork(t_philo *philo)
{
	if (philo->data->philos_count != 1 && philo->id == philo->data->philos_count
		- 1 && philo->id != 0)
		pthread_mutex_unlock(&philo->data->philos[0].left_fork);
	else if (philo->data->philos_count != 1)
		pthread_mutex_unlock(&philo->data->philos[philo->id + 1].left_fork);
}
