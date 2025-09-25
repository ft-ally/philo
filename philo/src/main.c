/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 00:52:15 by aalombro          #+#    #+#             */
/*   Updated: 2025/06/06 00:52:15 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;

	if (argc != 5 && argc != 6)
		return (write(2, "error", 5), 1);
	if (!init_data(&data, argv))
		return (1);
	if (data.philo_count == 1)
	{
		printf("%lld 1 has taken a fork\n", time_stamp(&data));
		usleep(data.die_time * 1000);
		printf("%lld 1 died\n", time_stamp(&data));
		return (1);
	}
	if (!init_philo(&data, &philo))
		return (1);
	if (!init_threads(&data, philo))
		return (1);
	cleanup_philo(&data, philo);
	return (0);
}
//addcleanup philo with some flags