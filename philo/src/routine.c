/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:10:43 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/30 14:17:27 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	odd_routine(t_philo *p)
{
	if (!monitor_check(p) || !take_own_fork(p))
		return (0);
	if (!monitor_check(p))
		return (pthread_mutex_unlock(&p->fork), 0);
	if (!take_next_fork(p))
		return (pthread_mutex_unlock(&p->fork), 0);
	if (!monitor_check(p))
		return (unlock_forks(p, 0), 0);
	if (!is_eating(p))
		return (unlock_forks(p, 0), 0);
	unlock_forks(p, 0);
	if (!monitor_check(p))
		return (0);
	if (!is_sleeping(p))
		return (0);
	if (!monitor_check(p))
		return (0);
	return (1);
}

static int	even_routine(t_philo *p)
{
	if (!monitor_check(p) || !take_next_fork(p))
		return (0);
	if (!monitor_check(p) || !take_own_fork(p))
		return (pthread_mutex_unlock(&p->next->fork), 0);
	if (!monitor_check(p) || !is_eating(p))
		return (unlock_forks(p, 1), 0);
	unlock_forks(p, 1);
	if (!monitor_check(p) || !is_sleeping(p))
		return (0);
	if (!monitor_check(p))
		return (0);
	return (1);
}

static int	not_finished_eating(t_philo *p)
{
	pthread_mutex_lock(&p->meals_eaten_mutex);
	if (p->data->nmeals != -1 && p->meals_eaten >= p->data->nmeals)
	{
		p->done = 1;
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		return (0);
	}
	pthread_mutex_unlock(&p->meals_eaten_mutex);
	return (1);
}

static int	routine_loop(t_philo *p)
{
	if (p->seat % 2 != 0)
	{
		if (!odd_routine(p))
			return (0);
	}
	else
	{
		if (!even_routine(p))
			return (0);
	}
	if (monitor_check(p))
	{
		ft_print(p, "is thinking");
		if (p->data->die_time <= p->data->eat_time + p->data->sleep_time + 10)
			usleep(5);
		else if (p->seat == p->data->philo_count)
			usleep(500);
		else if (p->seat % 2)
			usleep(200);
		else
			usleep(100);
	}
	else
		return (0);
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->seat % 2)
		usleep(1000);
	pthread_mutex_lock(&p->meal_time_mutex);
	p->last_meal_t = ft_gettime();
	pthread_mutex_unlock(&p->meal_time_mutex);
	while (not_finished_eating(p) && monitor_check(p))
	{
		if (!monitor_check(p))
			return (NULL);
		if (!routine_loop(p))
			return (NULL);
	}
	return (NULL);
}
