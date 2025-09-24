/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalombro <aalombro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 01:01:37 by aalombro          #+#    #+#             */
/*   Updated: 2025/06/06 01:01:37 by aalombro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct t_data
{
	long				philo_count;
	long				die_time;
	long				eat_time;
	long				sleep_time;
	long				nmeals;
	long				death;
	pthread_t			*threads;
	pthread_t			monitor;
	long long			start_time;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		print_mutex;
}	t_data;

typedef struct t_philo
{
	struct t_philo		*prev;
	int					seat;
	long				meals_eaten;
	long long			last_meal_t;
	int					done;
	pthread_mutex_t		fork;
	pthread_mutex_t		meal_time_mutex;
	pthread_mutex_t		meals_eaten_mutex;
	t_data				*data;
	struct t_philo		*next;
}	t_philo;

//init.c
int			parse_args(char **argv);
int			init_data(t_data *data, char **argv);
int			init_threads(t_data *data, t_philo *p);
void		init_philo_node(int i, t_data *data, t_philo *p);
int			init_philo(t_data *data, t_philo **p);
//monitor.c
void		*monitor_routine(void *arg);
//routine.c
void		*routine(void *arg);
//routine_utils.c
int			take_own_fork(t_philo *p);
int			take_next_fork(t_philo *p);
int			is_eating(t_philo *p);
void		unlock_forks(t_philo *p, int even);
int			is_sleeping(t_philo *p);
//free_utils.c
void		cleanup_philo(t_data *data, t_philo *p);

//utils.c
int			error_check(t_data *d);
int			monitor_check(t_philo *p);
long long	ft_gettime(void);
int			ft_sleep(t_philo *p, long long time);
void		ft_print(t_philo *p, char *str);
//utils_str.c
int			is_num(char c);
long		ft_atol(char *str);
long long	time_stamp(t_data *d);

#endif
