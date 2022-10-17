/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 16:15:50 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_deaths(struct philo_data *philo, struct timeval current_time)
{
	long	die_timestamp;
	long	current_timestamp;

	pthread_mutex_lock(&philo->data->lock_eat);
	die_timestamp = (philo->last_eat_time + philo->data->time_to_die);
	pthread_mutex_unlock(&philo->data->lock_eat);
	current_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec)
			/ 1000);
	if (die_timestamp < current_timestamp)
	{
		pthread_mutex_lock(&philo->data->lock_dead);
		philo->data->is_dead = 1;
		pthread_mutex_unlock(&philo->data->lock_dead);
		printf("%ld %d died\n", current_timestamp, philo->index);
		return (1);
	}
	return (0);
}

int	check_end(struct philo_data *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->number_of_philosophers)
	{
		pthread_mutex_lock(&philo->data->lock_eat);
		if (philo[i].nbr_of_eat < philo->data->nbr_philo_must_eat)
		{
			pthread_mutex_unlock(&philo->data->lock_eat);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->lock_eat);
		i++;
	}
	pthread_mutex_lock(&philo->data->lock_dead);
	philo->data->is_dead = 1;
	pthread_mutex_unlock(&philo->data->lock_dead);
	return (1);
}

void	check_philo_death(struct philo_data *philo)
{
	int				i;
	struct timeval	current_time;

	while (1)
	{
		i = 0;
		gettimeofday(&current_time, NULL);
		if (philo->data->nbr_philo_must_eat > 0
			&& check_end(philo))
			return ;
		while (i < philo->data->number_of_philosophers)
		{
			if (check_deaths(&philo[i], current_time))
				return ;
			i++;
		}
		usleep(100);
	}
}
