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

void	*thread_check_finished_philos(void *arg)
{
	t_data	*data;
	int		count;

	data = (t_data *)arg;
	count = 0;
	while (1)
	{
		sem_wait(data->sem_eat_complete);
		count++;
		if (count == data->number_of_philosophers)
			break ;
		sem_post(data->sem_pause);
	}
	sem_post(data->sem_kill_all);
	return (NULL);
}

void	*thread_exit_check(void *p)
{
	t_philo_data	*philo_data;
	struct timeval	current_time;
	long			die_timestamp;
	long			current_timestamp;

	philo_data = (t_philo_data *)p;
	while (1)
	{
		sem_wait(philo_data->data->sem_last_eat_time);
		die_timestamp = (philo_data->last_eat_time
				+ philo_data->data->time_to_die);
		sem_post(philo_data->data->sem_last_eat_time);
		gettimeofday(&current_time, NULL);
		current_timestamp = ((current_time.tv_sec * 1000000
					+ current_time.tv_usec) / 1000);
		if (die_timestamp < current_timestamp)
		{
			sem_wait(philo_data->data->sem_pause);
			gettimeofday(&current_time, NULL);
			printf("%ld %d died.\n", current_timestamp, philo_data->index);
			sem_post(philo_data->data->sem_kill_all);
			return (NULL);
		}
		usleep(100);
	}
}
