/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:09:53 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/26 13:04:29 by aalombro         ###   ########.fr       */
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

static int	check_death(t_philo *p)
{
	long long	current_time;

	pthread_mutex_lock(&p->meal_time_mutex);
	current_time = ft_gettime();
	if ((current_time - p->last_meal_t) > p->data->die_time)
	{
		pthread_mutex_lock(&p->data->death_mutex);
		if (p->data->death == 0)
		{
			ft_print(p, "died");
			p->data->death = 1;
		}
		pthread_mutex_unlock(&p->data->death_mutex);
		pthread_mutex_unlock(&p->meal_time_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&p->meal_time_mutex);
	return (0);
}

static int	check_philos(t_philo *p, t_philo *head, int finished)
{
	int	is_done;

	while (1)
	{
		usleep(50);
		pthread_mutex_lock(&p->meals_eaten_mutex);
		is_done = (p->data->nmeals != -1 && p->meals_eaten >= p->data->nmeals);
		pthread_mutex_unlock(&p->meals_eaten_mutex);
		if (!is_done && check_death(p) == -1)
			return (-1);
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
	int			finished_count;
	int			not_dead;

	finished_count = 0;
	p = (t_philo *)arg;
	not_dead = monitor_check(p);
	usleep(1000);
	while (not_dead)
	{
		finished_count = check_philos(p, p, finished_count);
		if (finished_count == -1)
			return (NULL);
		if (p->data->nmeals != -1 && finished_count == p->data->philo_count)
		{
			pthread_mutex_lock(&p->data->death_mutex);
			p->data->death = 1;
			pthread_mutex_unlock(&p->data->death_mutex);
			return (NULL);
		}
		not_dead = monitor_check(p);
	}
	return (NULL);
}
