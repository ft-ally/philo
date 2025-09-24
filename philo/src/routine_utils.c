/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:10:43 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/24 18:27:40 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_own_fork(t_philo *p)
{
	pthread_mutex_lock(&p->fork);
	if (!monitor_check(p))
	{
		pthread_mutex_unlock(&p->fork);
		return (0);
	}
	ft_print(p, "has taken a fork");
	return (1);
}

int	take_next_fork(t_philo *p)
{
	pthread_mutex_lock(&p->next->fork);
	if (!monitor_check(p))
	{
		pthread_mutex_unlock(&p->next->fork);
		return (0);
	}
	ft_print(p, "has taken a fork");
	return (1);
}

int	is_eating(t_philo *p)
{
	if (!monitor_check(p))
		return (0);

	pthread_mutex_lock(&p->meal_time_mutex);
	p->last_meal_t = ft_gettime();
	pthread_mutex_unlock(&p->meal_time_mutex);
	pthread_mutex_lock(&p->meals_eaten_mutex);
	if (p->data->nmeals != -1 && p->meals_eaten >= p->data->nmeals)
	{
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		return (0);
	}
	p->meals_eaten++;
	pthread_mutex_unlock(&p->meals_eaten_mutex);
	ft_print(p, "is eating");
	if (!ft_sleep(p, p->data->eat_time))
		return (0);
	return (1);
}

void	unlock_forks(t_philo *p, int even)
{
	if (!even)
	{
		pthread_mutex_unlock(&p->next->fork);
		pthread_mutex_unlock(&p->fork);
	}
	else
	{
		pthread_mutex_unlock(&p->fork);
		pthread_mutex_unlock(&p->next->fork);
	}
}

int	is_sleeping(t_philo *p)
{
	if (!monitor_check(p))
		return (0);
	ft_print(p, "is sleeping");
	if (!ft_sleep(p, p->data->sleep_time))
		return (0);
	return (1);
}
