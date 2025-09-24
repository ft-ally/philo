/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:07:35 by aalombro          #+#    #+#             */
/*   Updated: 2025/07/26 15:25:52 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_threads(t_data *data, t_philo *p)
{
	int			i;
	t_philo		*tmp;

	tmp = p;
	i = 0;
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	if (!data->threads)
		return (0);
	while (i < data->philo_count)
	{
		if (pthread_create(&data->threads[i], NULL, routine, tmp) != 0)
			return (0);
		tmp = tmp->next;
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, p) != 0)
		return (0);
	return (1);
}

void	init_philo_node(int i, t_data *data, t_philo *p)
{
	p->seat = i + 1;
	p->meals_eaten = 0;
	p->done = -1;
	p->data = data;
	p->last_meal_t = ft_gettime();
	pthread_mutex_init(&p->fork, NULL);
	pthread_mutex_init(&p->meal_time_mutex, NULL);
	pthread_mutex_init(&p->meals_eaten_mutex, NULL);
}

int	init_philo(t_data *data, t_philo **p)
{
	int		i;
	t_philo	*new_philo;
	t_philo	*first;

	i = 0;
	while (i < data->philo_count)
	{
		new_philo = malloc(sizeof(t_philo));
		if (!new_philo)
			return (0);
		init_philo_node(i, data, new_philo);
		if (i == 0)
			first = new_philo;
		else
		{
			new_philo->prev = *p;
			(*p)->next = new_philo;
		}
		*p = new_philo;
		i++;
	}
	(*p)->next = first;
	first->prev = *p;
	*p = first;
	return (1);
}

int	parse_args(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][j] == '-' || argv[i][j] == '+')
			j++;
		if (!argv[i][j])
			return (0);
		while (argv[i][j])
		{
			if (!is_num(argv[i][j]))
				return (write(2, "error", 5), 0);
			j++;
		}
		i++;
	}
	return (1);
}

int	init_data(t_data *data, char **argv)
{
	if (!parse_args(argv))
		return (0);
	else
	{
		data->philo_count = ft_atol(argv[1]);
		data->die_time = ft_atol(argv[2]);
		data->eat_time = ft_atol(argv[3]);
		data->sleep_time = ft_atol(argv[4]);
		data->death = 0;
		data->start_time = ft_gettime();
		pthread_mutex_init(&data->print_mutex, NULL);
		pthread_mutex_init(&data->death_mutex, NULL);
		if (argv[5])
		{
			data->nmeals = ft_atol(argv[5]);
			if (data->nmeals <= 0)
				return (write(2, "error", 5), 0);
		}
		else
			data->nmeals = -1;
	}
	if (!error_check(data))
		return (0);
	return (1);
}
