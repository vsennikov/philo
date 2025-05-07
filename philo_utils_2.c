/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:49:42 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/07 12:00:18 by vsenniko         ###   ########.fr       */
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
		pthread_mutex_destroy(&data->philos[i].dead_lock);
		pthread_mutex_destroy(&data->philos[i].instance_lock);
		i++;
	}
	i = 0;
	free(data->philos);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->finished_lock);
	pthread_mutex_destroy(&data->instance_lock);
	free(data);
}

void	custom_usleep(long long sleep)
{
	long long	start_time;

	start_time = current_time();
	while ((current_time() - start_time) * 1000 < sleep)
		usleep(500);
}

static int	ft_isspace(char ch)
{
	if ((ch >= '\t' && ch <= '\r') || ch == ' ')
		return (1);
	return (0);
}

static long	ft_atoi(const char *str)
{
	int		i;
	int		minus;
	long	result;

	i = 0;
	minus = 0;
	result = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		minus = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	if (minus)
		result *= -1;
	return (result);
}

int	atoi_wrap(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	if (i > 10)
		return (-1);
	else if (i == 10)
	{
		if (ft_atoi(str) > 2147483647 || ft_atoi(str) < 1)
			return (-1);
	}
	if (ft_atoi(str) < 1)
		return (-1);
	return (ft_atoi(str));
}
