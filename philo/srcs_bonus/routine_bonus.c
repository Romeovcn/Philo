/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/19 17:38:18 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	go_to_sleep(long timestamp_to_wait)
{
	struct timeval	ct;

	while (1)
	{
		gettimeofday(&ct, NULL);
		if (get_time_stamp(ct) >= timestamp_to_wait)
			return ;
		usleep(100);
	}
	return ;
}

void	philo_take_fork(t_philo_data *philo_data)
{
	struct timeval	ct;
	long			time_stamp;

	sem_wait(philo_data->data->sem_fork);
	sem_wait(philo_data->data->sem_pause);
	gettimeofday(&ct, NULL);
	time_stamp = get_time_stamp(ct) - philo_data->data->start_timestamp;
	printf("%ld %d has taken a fork\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
}

void	philo_eat(t_philo_data *philo_data, int *eating_counter)
{
	struct timeval	ct;
	long			time_stamp;

	gettimeofday(&ct, NULL);
	time_stamp = get_time_stamp(ct);
	sem_wait(philo_data->data->sem_last_eat_time);
	philo_data->last_eat_time = time_stamp;
	sem_post(philo_data->data->sem_last_eat_time);
	sem_wait(philo_data->data->sem_pause);
	printf("%ld %d is eating\n", time_stamp - philo_data->data->start_timestamp,
		philo_data->index);
	sem_post(philo_data->data->sem_pause);
	(*eating_counter)++;
	if (*eating_counter == philo_data->data->nbr_philo_must_eat)
	{
		sem_wait(philo_data->data->sem_pause);
		sem_post(philo_data->data->sem_eat_complete);
	}
	go_to_sleep(time_stamp + philo_data->data->time_to_eat);
}

void	philo_sleep(t_philo_data *philo_data)
{
	struct timeval	ct;
	long			time_stamp;

	gettimeofday(&ct, NULL);
	time_stamp = get_time_stamp(ct) - philo_data->data->start_timestamp;
	sem_wait(philo_data->data->sem_pause);
	printf("%ld %d is sleeping\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
	go_to_sleep(get_time_stamp(ct) + philo_data->data->time_to_sleep);
	gettimeofday(&ct, NULL);
	time_stamp = get_time_stamp(ct) - philo_data->data->start_timestamp;
	sem_wait(philo_data->data->sem_pause);
	printf("%ld %d is thinking\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
	if (philo_data->data->time_to_sleep == 0)
		usleep(1000);
}

void	philo_routine(t_philo_data *philo_data, int *eating_counter)
{
	struct timeval	ct;

	philo_take_fork(philo_data);
	philo_take_fork(philo_data);
	gettimeofday(&ct, NULL);
	philo_eat(philo_data, eating_counter);
	sem_post(philo_data->data->sem_fork);
	sem_post(philo_data->data->sem_fork);
	philo_sleep(philo_data);
}
