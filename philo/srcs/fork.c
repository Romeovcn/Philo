/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/10 19:01:56 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	drop_left_fork(t_philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	lst->previous->right_fork = 1;
	lst->left_fork = 1;
}

void	drop_right_fork(t_philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	lst->previous->left_fork = 1;
	lst->right_fork = 1;
}

int	take_left_fork(t_philo_list *lst, int index, t_philo_data *philo)
{
	struct timeval	ct;
	long			time_stamp;

	while (1)
	{
		pthread_mutex_lock(&philo->data->lock_dead);
		if (philo->data->is_dead)
			return (pthread_mutex_unlock(&philo->data->lock_dead), -1);
		pthread_mutex_lock(&philo->data->lock_fork);
		if (lst->left_fork)
		{
			gettimeofday(&ct, NULL);
			time_stamp = ((ct.tv_sec * 1000000 + ct.tv_usec) / 1000)
				- philo->data->start_timestamp;
			printf("%ld %d has taken a fork\n", time_stamp, index);
			lst->previous->right_fork = 0;
			lst->left_fork = 0;
			return (pthread_mutex_unlock(&philo->data->lock_fork),
				pthread_mutex_unlock(&philo->data->lock_dead),
				1);
		}
		pthread_mutex_unlock(&philo->data->lock_fork);
		pthread_mutex_unlock(&philo->data->lock_dead);
		usleep(100);
	}
}

int	take_right_fork(t_philo_list *lst, int index, t_philo_data *philo)
{
	struct timeval	ct;
	long			time_stamp;

	while (1)
	{
		pthread_mutex_lock(&philo->data->lock_dead);
		if (philo->data->is_dead)
			return (pthread_mutex_unlock(&philo->data->lock_dead), -1);
		pthread_mutex_lock(&philo->data->lock_fork);
		if (lst->right_fork)
		{
			gettimeofday(&ct, NULL);
			time_stamp = ((ct.tv_sec * 1000000 + ct.tv_usec) / 1000)
				- philo->data->start_timestamp;
			printf("%ld %d has taken a fork\n", time_stamp, index);
			lst->next->left_fork = 0;
			lst->right_fork = 0;
			return (pthread_mutex_unlock(&philo->data->lock_fork),
				pthread_mutex_unlock(&philo->data->lock_dead),
				1);
		}
		pthread_mutex_unlock(&philo->data->lock_fork);
		pthread_mutex_unlock(&philo->data->lock_dead);
		usleep(100);
	}
}
