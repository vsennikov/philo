/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:53:53 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/12 15:09:03 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_and_print(int flag, t_data *data)
{
	if (flag == NOT_VALID_ERR)
		return (ft_putstr_fd(NON_VALID_INPUT, 2), 0);
	else if (flag == CALLOC_ERR)
		return (ft_putstr_fd(ALLOC_PHILOS, 2), 0);
	else if (flag == DEAD_MUT_ERR)
		return (ft_putstr_fd(DEAD_MUTEXS, 2), 0);
	else if (flag == PRINT_MUT_ERR)
		return (free(data->philos), ft_putstr_fd(PRINTING_MUTEXS, 2), 0);
	else if (flag == FINISH_MUT_ERR)
		return (free(data->philos), pthread_mutex_destroy(&data->print_lock),
			ft_putstr_fd(FINISHED_MUTEX, 2), 0);
	else if (flag == D_INST_LOCK)
		return (free(data->philos), pthread_mutex_destroy(&data->print_lock),
			pthread_mutex_destroy(&data->finished_lock),
			ft_putstr_fd(FINISHED_MUTEX, 2), 0);
	else if (flag == DATA_ERR)
		return (ft_putstr_fd(ALLOC_GENERAL, 2), 1);
	else if (flag == L_FORK_ERR)
		return (ft_putstr_fd(FORK_MUTEXS, 2), 0);
	else if (flag == MEAL_ERR)
		return (ft_putstr_fd(MEAL_MUTEX, 2), 0);
	return (1);
}

void	clean_init(t_data *data, int t_created)
{
	int	i;

	i = 0;
	while (i != t_created)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i != data->philos_count)
	{
		pthread_mutex_destroy(&data->philos[i].left_fork);
		pthread_mutex_destroy(&data->philos[i].meal_lock);
		pthread_mutex_destroy(&data->philos[i].dead_lock);
		pthread_mutex_destroy(&data->philos[i].instance_lock);
		i++;
	}
	free(data->philos);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->finished_lock);
	pthread_mutex_destroy(&data->instance_lock);
	free(data);
}
