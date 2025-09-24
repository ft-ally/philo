/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:09:53 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/24 15:45:50 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_meals(t_philo *p)
{
	pthread_mutex_lock(&p->meals_eaten_mutex);
	if (p->data->nmeals != -1 && (p->meals_eaten >= p->data->nmeals)
		&& p->done == -1)
	{
		p->done = 1;
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		return (1);
	}
	pthread_mutex_unlock(&p->meals_eaten_mutex);
	return (0);
}

static int	check_philos(t_philo *p, t_philo *head, int finished,
	long long current_time)
{
	while (1)
	{
		usleep(100);
		pthread_mutex_lock(&p->meals_eaten_mutex);
		int is_done = (p->data->nmeals != -1 && p->meals_eaten >= p->data->nmeals);
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		
		if (!is_done)
		{
			pthread_mutex_lock(&p->meal_time_mutex);
			current_time = ft_gettime();
			if ((current_time - p->last_meal_t) > p->data->die_time)
			{
				pthread_mutex_lock(&p->data->death_mutex);
				if (p->data->death == 0)
				{
					p->data->death = 1;
					ft_print(p, "died");
				}
				pthread_mutex_unlock(&p->data->death_mutex);
				pthread_mutex_unlock(&p->meal_time_mutex);
				return (-1);
			}
			pthread_mutex_unlock(&p->meal_time_mutex);
		}
		finished += check_meals(p);
		p = p->next;
		if (p == head)
			break ;
	}
	return (finished);
}

void	*monitor_routine(void *arg)
{
	t_philo		*p;
	long long	current_time;
	int			finished_count;
	int			not_dead;

	finished_count = 0;
	current_time = 0;
	p = (t_philo *)arg;
	not_dead = death_check(p);
	usleep(1000);
	while (not_dead)
	{
		finished_count = check_philos(p, p, finished_count, current_time);
		if (finished_count == -1)
			return (NULL);
		if (p->data->nmeals != -1 && finished_count == p->data->philo_count)
		{
			pthread_mutex_lock(&p->data->death_mutex);
			p->data->death = 1;
			pthread_mutex_unlock(&p->data->death_mutex);
			return (NULL);
		}
		not_dead = death_check(p);
	}
	return (NULL);
}
