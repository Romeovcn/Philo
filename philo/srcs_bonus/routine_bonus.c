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

void	philo_take_fork(t_philo_data *philo_data)
{
	struct timeval	ct;
	long			time_stamp;

	sem_wait(philo_data->data->sem_fork);
	sem_wait(philo_data->data->sem_pause);
	gettimeofday(&ct, NULL);
	time_stamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000
		- philo_data->data->start_timestamp;
	printf("%ld %d take fork\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
}

void	philo_eat(t_philo_data *philo_data, int *eating_counter)
{
	struct timeval	ct;
	long			time_stamp;

	gettimeofday(&ct, NULL);
	time_stamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000;
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
	usleep(philo_data->data->time_to_eat * 1000);
}

void	philo_sleep(t_philo_data *philo_data)
{
	struct timeval	ct;
	long			time_stamp;

	gettimeofday(&ct, NULL);
	time_stamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000
		- philo_data->data->start_timestamp;
	sem_wait(philo_data->data->sem_pause);
	printf("%ld %d is sleeping\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
	usleep(philo_data->data->time_to_sleep * 1000);
	gettimeofday(&ct, NULL);
	time_stamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000
		- philo_data->data->start_timestamp;
	sem_wait(philo_data->data->sem_pause);
	printf("%ld %d is thinking\n", time_stamp, philo_data->index);
	sem_post(philo_data->data->sem_pause);
}

void	routine(t_philo_data *philo_data, int *eating_counter)
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
