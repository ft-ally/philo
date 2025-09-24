/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:10:43 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/24 18:41:16 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	odd_routine(t_philo *p)
{
	if (!monitor_check(p))
		return (0);
	if (!take_own_fork(p))
		return (0);
	if (!monitor_check(p))
		return (0);
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
	if (!monitor_check(p))
		return (0);
	if (!take_next_fork(p))
		return (0);
	if (!monitor_check(p))
		return (0);
	if (!take_own_fork(p))
		return (pthread_mutex_unlock(&p->next->fork), 0);
	if (!monitor_check(p))
		return (unlock_forks(p, 1), 0);
	if (!is_eating(p))
		return (unlock_forks(p, 1), 0);
	unlock_forks(p, 1);
	if (!monitor_check(p))
		return (0);
	if (!is_sleeping(p))
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

void	*routine(void *arg)
{
	t_philo	*p;
	int		code;

	p = (t_philo *)arg;
	code = 1;
	if (p->seat % 2)
	{
		usleep(1000);
		pthread_mutex_lock(&p->meal_time_mutex);
		p->last_meal_t = ft_gettime();
		pthread_mutex_unlock(&p->meal_time_mutex);
	}
	while (code && not_finished_eating(p) && monitor_check(p))
	{
		if (!(code = monitor_check(p)))
			return (NULL);
		if (p->seat % 2 != 0)
		{
			if (!odd_routine(p))
				return (NULL);
		}
		else
		{
			if (!even_routine(p))
				return (NULL);
		}
		if (code && monitor_check(p))
		{
			ft_print(p, "is thinking");
			usleep(10);
		}
	}
	return (NULL);
}
