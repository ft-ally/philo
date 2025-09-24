/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:26:40 by aalombro          #+#    #+#             */
/*   Updated: 2025/07/26 16:49:01 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

static void	destroy_philo_mutex(t_philo *tmp)
{
	pthread_mutex_destroy(&tmp->fork);
	pthread_mutex_destroy(&tmp->meal_time_mutex);
	pthread_mutex_destroy(&tmp->meals_eaten_mutex);
}

static void	destroy_data_mutex(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

void	cleanup_philo(t_data *data, t_philo *p)
{
	t_philo	*tmp;
	t_philo	*to_free;
	int		i;

	tmp = p;
	i = 0;
	join_threads(data);
	while (i < data->philo_count)
	{
		destroy_philo_mutex(tmp);
		tmp = tmp->next;
		i++;
	}
	i = 0;
	tmp = p;
	while (i < data->philo_count)
	{
		to_free = tmp;
		tmp = tmp->next;
		free(to_free);
		i++;
	}
	destroy_data_mutex(data);
	free(data->threads);
}
