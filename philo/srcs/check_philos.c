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

int	check_deaths(t_philo_data *philo)
{
	long			die_timestamp;
	long			current_timestamp;
	struct timeval	ct;

	pthread_mutex_lock(&philo->data->lock_eat);
	die_timestamp = (philo->last_eat_time + philo->data->time_to_die);
	pthread_mutex_unlock(&philo->data->lock_eat);
	gettimeofday(&ct, NULL);
	current_timestamp = ((ct.tv_sec * 1000000 + ct.tv_usec)
			/ 1000);
	if (die_timestamp < current_timestamp)
	{
		pthread_mutex_lock(&philo->data->lock_dead);
		philo->data->is_dead = 1;
		pthread_mutex_unlock(&philo->data->lock_dead);
		printf("%ld %d died\n", die_timestamp - philo->data->start_timestamp,
			philo->index);
		return (1);
	}
	return (0);
}

int	check_end(t_philo_data *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->philos_nbr)
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

void	check_philo_death(t_philo_data *philo)
{
	int				i;

	while (1)
	{
		i = 0;
		if (philo->data->nbr_philo_must_eat > 0
			&& check_end(philo))
			return ;
		while (i < philo->data->philos_nbr)
		{
			if (check_deaths(&philo[i]))
				return ;
			i++;
		}
		usleep(100);
	}
}
