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
	struct timeval	ct;

	while (1)
	{
		gettimeofday(&ct, NULL);
		if (check_dead(data))
			return ;
		if (get_time_stamp(ct) >= timestamp_to_wait)
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
	long	time_stamp;

	time_stamp = get_time_stamp(current_time) - philo->data->start_timestamp;
	pthread_mutex_lock(&philo->data->lock_dead);
	if (philo->data->is_dead)
		return (pthread_mutex_unlock(&philo->data->lock_dead), 1);
	printf("%ld %d %s", time_stamp, philo->index, message);
	pthread_mutex_unlock(&philo->data->lock_dead);
	return (0);
}

long	get_time_stamp(struct timeval current_time)
{
	return ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
}
