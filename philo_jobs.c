/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_jobs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:18:12 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/08 14:35:55 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			check_finished(t_philo *philo);
static int	p_eat(t_philo *philo);
static int	p_sleep(t_philo *philo);
static int	p_think(t_philo *philo);

// sometimes (like 1/22) i have problem with 199 600 200 200. 
//Philo 199 should die at 600,
//	but he eats and everything is good.
// during check it looks like now 599,
//	but its actually 600 according to timestamp before and after.
// what could give me wrong result????
// i change behavior of last_eaten and start_time
// now start time lcoking after finishing initialization
// and last_eaten by defolt is 0 and never cheked before first eating
// Should rewrite it because they can be dead
// I should put it somewhere in philo_loop before actual lool,
//	not assigne to 0 and thats all
void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->instance_lock);
	pthread_mutex_lock(&philo->data->instance_lock);
	philo->last_eaten = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->instance_lock);
	pthread_mutex_unlock(&philo->instance_lock);
	if (!philo->id && philo->philos_count != 1 && philo->philos_count != 3
		&& philo->philos_count % 2 != 0)
		custom_usleep(philo->time_to_eat * 1000);
	while (1)
	{
		if (check_finished(philo))
			return (NULL);
		if (!p_eat(philo))
			return (NULL);
		if (check_finished(philo))
			return (NULL);
		if (!p_sleep(philo))
			return (NULL);
		if (check_finished(philo))
			return (NULL);
		if (!p_think(philo))
			return (NULL);
	}
	return (NULL);
}

static int	p_eat(t_philo *philo)
{
	static int	numb_of_meals = 0;

	if (check_finished(philo))
		return (0);
	if (!numb_of_meals)
		set_meal_num(&numb_of_meals, philo);
	if (!take_forks(philo))
		return (0);
	if (check_finished(philo))
		return (leave_forks(philo), 0);
	if (philo_died(philo))
		return (leave_forks(philo), 0);
	// pthread_mutex_lock(&philo->instance_lock);
	// printf("cur_time = %lld\n", current_time());
	// printf("last eaten = %lld\n", philo->last_eaten);
	// printf("the diff = %lld\n", current_time() - philo->last_eaten);
	philo->last_eaten = current_time();
	// pthread_mutex_unlock(&philo->instance_lock);
	print_state(philo->data, philo->id + 1, "is eating");
	if (!check_during_eat(philo))
		return (0);
	if (check_finished(philo))
		return (leave_forks(philo), 0);
	leave_forks(philo);
	if (philo->numb_of_meals != -2)
		if (update_meals_eaten(philo))
			return (0);
	return (1);
}

static int	p_sleep(t_philo *philo)
{
	if (check_finished(philo))
		return (0);
	print_state(philo->data, philo->id + 1, "is sleeping");
	custom_usleep(philo->time_to_sleep * 1000);
	return (1);
}

static int	p_think(t_philo *philo)
{
	int			id;
	static int	n_philos = 0;
	int			delay;

	if (check_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->data->instance_lock);
	id = philo->data->id_to_delay;
	update_id_delay(philo);
	if (!n_philos)
		n_philos = philo->data->philos_count;
	pthread_mutex_unlock(&philo->data->instance_lock);
	print_state(philo->data, philo->id + 1, "is thinking");
	delay = 0;
	if (philo->id == id && philo->philos_count % 2 != 0)
	{
		if (philo->time_to_eat > philo->time_to_sleep)
			delay = philo->time_to_eat - philo->time_to_sleep;
		custom_usleep(delay + 20000);
	}
	else
		custom_usleep(1000);
	return (1);
}

int	check_finished(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->finished_lock);
	if (philo->data->finished)
		return (pthread_mutex_unlock(&philo->data->finished_lock), 1);
	pthread_mutex_unlock(&philo->data->finished_lock);
	return (0);
}
