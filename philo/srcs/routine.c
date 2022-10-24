/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 17:48:04 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo_data *philo)
{
	struct timeval	ct;

	gettimeofday(&ct, NULL);
	print_action(philo, "is eating\n", ct);
	pthread_mutex_lock(&philo->data->lock_eat);
	philo->last_eat_time = get_time_stamp(ct);
	philo->nbr_of_eat++;
	pthread_mutex_unlock(&philo->data->lock_eat);
	go_to_sleep(get_time_stamp(ct) + (*philo).data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->lock_fork);
	drop_left_fork(philo->fork_table, philo->index);
	drop_right_fork(philo->fork_table, philo->index);
	pthread_mutex_unlock(&philo->data->lock_fork);
}

void	philo_sleep(t_philo_data *philo)
{
	struct timeval	ct;

	gettimeofday(&ct, NULL);
	print_action(philo, "is sleeping\n", ct);
	go_to_sleep(get_time_stamp(ct) + (*philo).data->time_to_sleep, philo->data);
	gettimeofday(&ct, NULL);
	print_action(philo, "is thinking\n", ct);
	if (philo->data->time_to_sleep == 0)
		usleep(1000);
}

int	philo_routine(t_philo_data *philo)
{
	int	left_fork;
	int	right_fork;

	while (philo->fork_table->index != philo->index)
		philo->fork_table = philo->fork_table->next;
	left_fork = take_left_fork(philo->fork_table, philo->index, philo);
	right_fork = take_right_fork(philo->fork_table, philo->index, philo);
	if (left_fork && right_fork)
	{
		philo_eat(philo);
		philo_sleep(philo);
	}
	if (left_fork == -1 || right_fork == -1)
		return (1);
	return (0);
}
