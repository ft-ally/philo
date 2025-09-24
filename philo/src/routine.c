/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:10:43 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/24 16:24:08 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	odd_routine(t_philo *p)
{
	if (!death_check(p))
		return (0);
	if (!take_own_fork(p))
		return (0);
	if (!death_check(p))
		return (0);
	if (!take_next_fork(p))
	{
		pthread_mutex_unlock(&p->fork);
		return (0);
	}
	if (!death_check(p))
	{
		unlock_forks(p, 0);
		return (0);
	}
	if (!is_eating(p))
	{
		unlock_forks(p, 0);
		return (0);
	}
	unlock_forks(p, 0);
	if (!death_check(p))
		return (0);
	if (!is_sleeping(p))
		return (0);
	if (!death_check(p))
		return (0);
	return (1);
}

static int	even_routine(t_philo *p)
{
	if (!death_check(p))
		return (0);
	if (!take_next_fork(p))
		return (0);
	if (!death_check(p))
		return (0);
	if (!take_own_fork(p))
	{
		pthread_mutex_unlock(&p->next->fork);
		return (0);
	}
	if (!death_check(p))
	{
		unlock_forks(p, 1);
		return (0);
	}
	if (!is_eating(p))
	{
		unlock_forks(p, 1);
		return (0);
	}
	unlock_forks(p, 1);
	if (!death_check(p))
		return (0);
	if (!is_sleeping(p))
		return (0);
	if (!death_check(p))
		return (0);
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*p;
	int		not_dead;

	p = (t_philo *)arg;
	not_dead = 1;
	if (p->seat % 2)
		usleep(50 * p->seat);
	while (not_dead)
	{
		not_dead = death_check(p);
		if (!death_check(p))
			return (NULL);
			
		pthread_mutex_lock(&p->meals_eaten_mutex);
		if (p->data->nmeals != -1 && p->meals_eaten >= p->data->nmeals)
		{
			p->done = 1;
			pthread_mutex_unlock(&p->meals_eaten_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		
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
		not_dead = death_check(p);
		ft_print(p, "is thinking");
	}
	return (NULL);
}
