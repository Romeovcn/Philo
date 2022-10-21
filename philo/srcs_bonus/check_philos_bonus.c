/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philos_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/19 17:35:42 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*philos_eat_check_func(void *arg)
{
	t_data	*data;
	int		count;

	data = (t_data *)arg;
	count = 0;
	while (1)
	{
		sem_wait(data->sem_eat_complete);
		count++;
		if (count == data->philos_nbr)
			break ;
		sem_post(data->sem_pause);
	}
	sem_post(data->sem_kill_all);
	return (NULL);
}

void	*death_check_func(void *p)
{
	t_philo_data	*philo_data;
	struct timeval	ct;
	long			die_time;
	long			current_time;

	philo_data = (t_philo_data *)p;
	while (1)
	{
		sem_wait(philo_data->data->sem_last_eat_time);
		die_time = philo_data->last_eat_time + philo_data->data->time_to_die;
		sem_post(philo_data->data->sem_last_eat_time);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1000000 + ct.tv_usec) / 1000);
		if (die_time < current_time)
		{
			sem_wait(philo_data->data->sem_pause);
			printf("%ld %d died.\n", current_time
				- philo_data->data->start_timestamp, philo_data->index);
			sem_post(philo_data->data->sem_kill_all);
			return (NULL);
		}
		usleep(100);
	}
}
