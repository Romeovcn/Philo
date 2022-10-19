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

void	post_message(t_philo_data philo_data, char *message, long time_to_wait)
{
	struct timeval	current_time;

	sem_wait(philo_data.data->sem_pause);
	gettimeofday(&current_time, NULL);
	printf("%ld %d %s\n", ((current_time.tv_sec * 1000000
				+ current_time.tv_usec) / 1000), philo_data.index, message);
	sem_post(philo_data.data->sem_pause);
	usleep(time_to_wait);
}

void	take_fork(t_philo_data *philo_data)
{
	struct timeval	current_time;

	sem_wait(philo_data->data->sem_fork);
	sem_wait(philo_data->data->sem_pause);
	gettimeofday(&current_time, NULL);
	printf("%ld %d take fork\n", ((current_time.tv_sec * 1000000
				+ current_time.tv_usec) / 1000), philo_data->index);
	sem_post(philo_data->data->sem_pause);
}

void	routine(t_philo_data *philo_data, int *eating_counter)
{
	struct timeval	current_time;

	take_fork(philo_data);
	take_fork(philo_data);
	gettimeofday(&current_time, NULL);
	sem_wait(philo_data->data->sem_last_eat_time);
	philo_data->last_eat_time = ((current_time.tv_sec * 1000000
				+ current_time.tv_usec) / 1000);
	sem_post(philo_data->data->sem_last_eat_time);
	post_message(*philo_data, "is eating", philo_data->data->time_to_eat
		* 1000);
	(*eating_counter)++;
	if (*eating_counter == philo_data->data->nbr_philo_must_eat)
	{
		sem_wait(philo_data->data->sem_pause);
		sem_post(philo_data->data->sem_eat_complete);
	}
	sem_post(philo_data->data->sem_fork);
	sem_post(philo_data->data->sem_fork);
	post_message(*philo_data, "is sleeping", philo_data->data->time_to_sleep
		* 1000);
	post_message(*philo_data, "is thinking", 0);
}
