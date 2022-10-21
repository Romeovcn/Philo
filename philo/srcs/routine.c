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

void	start_routine(t_philo_data *philo)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	print_action(philo, "is eating\n", current_time);
	pthread_mutex_lock(&philo->data->lock_eat);
	philo->last_eat_time = ((current_time.tv_sec * 1000000
				+ current_time.tv_usec) / 1000);
	philo->nbr_of_eat++;
	pthread_mutex_unlock(&philo->data->lock_eat);
	go_to_sleep(((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000)
		+ (*philo).data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->lock_fork);
	drop_left_fork(philo->fork_table, philo->index);
	drop_right_fork(philo->fork_table, philo->index);
	pthread_mutex_unlock(&philo->data->lock_fork);
	gettimeofday(&current_time, NULL);
	print_action(philo, "is sleeping\n", current_time);
	go_to_sleep(((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000)
		+ (*philo).data->time_to_eat, philo->data);
	gettimeofday(&current_time, NULL);
	print_action(philo, "is thinking\n", current_time);
}

int	init_routine(t_philo_data *philo)
{
	int	left_fork;
	int	right_fork;

	while (philo->fork_table->index != philo->index)
		philo->fork_table = philo->fork_table->next;
	left_fork = take_left_fork(philo->fork_table, philo->index, philo);
	right_fork = take_right_fork(philo->fork_table, philo->index, philo);
	if (left_fork && right_fork)
		start_routine(philo);
	if (left_fork == -1 || right_fork == -1)
		return (1);
	return (0);
}

void	*philo_thread_func(void *p)
{
	t_philo_data	*philo;

	philo = (t_philo_data *)p;
	if ((*philo).index % 2 == 1)
		usleep(10000);
	while (1)
	{
		if (init_routine(philo))
			break ;
	}
	return (NULL);
}

int	init_threads(t_data *data, t_philo_list *fork_table,
		pthread_t *philo_thread, t_philo_data *philo_data)
{
	struct timeval	ct;
	int				i;

	i = 0;
	gettimeofday(&ct, NULL);
	data->start_timestamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000;
	while (++i <= data->philos_nbr)
	{
		philo_data[i - 1].fork_table = fork_table;
		philo_data[i - 1].data = data;
		philo_data[i - 1].index = i;
		philo_data[i - 1].nbr_of_eat = 0;
		philo_data[i - 1].last_eat_time = ((ct.tv_sec * 1000000
					+ ct.tv_usec) / 1000);
		if (pthread_create(&philo_thread[i - 1], NULL, philo_thread_func,
				&philo_data[i - 1]) != 0)
		{
			pthread_mutex_lock(&data->lock_dead);
			data->is_dead = 1;
			pthread_mutex_unlock(&data->lock_dead);
			thread_error_join(philo_thread, i);
			return (printf("Error while initializing philo %d\n", i), 1);
		}
	}
	return (0);
}

void	join_threads(pthread_t *philo_thread, int philos_nbr)
{
	int	i;

	i = 1;
	while (i <= philos_nbr)
	{
		pthread_join(philo_thread[i - 1], NULL);
		i++;
	}
}
