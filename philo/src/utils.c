/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:05:46 by aalombro          #+#    #+#             */
/*   Updated: 2025/09/25 17:08:28 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	ft_sleep(t_philo *p, long long time)
{
	long long	start_time;
	long long	current_time;

	start_time = ft_gettime();
	while (1)
	{
		current_time = ft_gettime();
		if ((current_time - start_time) >= time)
			break ;
		if (!monitor_check(p))
			return (0);
		usleep(100);
	}
	return (1);
}

long long	ft_gettime(void)
{
	struct timeval	tv;
	long			time;

	time = gettimeofday(&tv, NULL);
	if (time == -1)
		return (write(2, "gettime error", 13), 1);
	return ((((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000));
}

int	error_check(t_data *d)
{
	if (d->philo_count <= 0 || d->die_time <= 0
		|| d->eat_time <= 0 || d->sleep_time <= 0)
		return (write(2, "error", 5), 0);
	return (1);
}

int	monitor_check(t_philo *p)
{
	int	res;

	res = 1;
	pthread_mutex_lock(&p->data->death_mutex);
	if (p->data->death == 1)
		res = 0;
	pthread_mutex_unlock(&p->data->death_mutex);
	return (res);
}

void	ft_print(t_philo *p, char *str)
{
	// if (!monitor_check(p))
	// 	return ;			//check this part, its to stop printing 
	pthread_mutex_lock(&p->data->print_mutex);
	printf("%lld %d %s\n", time_stamp(p->data), p->seat, str);
	pthread_mutex_unlock(&p->data->print_mutex);
}
