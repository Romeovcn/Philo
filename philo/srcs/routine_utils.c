/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/12 18:51:41 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_to_sleep(long timestamp_to_wait, t_data *data)
{
	struct timeval	current_time;

	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (check_dead(data))
			return ;
		if (((current_time.tv_sec * 1000000 + current_time.tv_usec)
				/ 1000) >= timestamp_to_wait)
			return ;
		usleep(100);
	}
	return ;
}

int	check_dead(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->lock_dead);
	result = data->is_dead;
	pthread_mutex_unlock(&data->lock_dead);
	return (result);
}

int	print_action(t_philo_data *philo, char *message,
		struct timeval current_time)
{
	pthread_mutex_lock(&philo->data->lock_dead);
	if (!philo->data->is_dead)
		printf("%ld %d %s", ((current_time.tv_sec * 1000000
					+ current_time.tv_usec) / 1000), philo->index, message);
	else
	{
		pthread_mutex_unlock(&philo->data->lock_dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->lock_dead);
	return (0);
}
