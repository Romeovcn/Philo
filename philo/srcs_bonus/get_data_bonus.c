/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/19 17:19:10 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_data(t_data *data, char **argv)
{
	sem_unlink("sem_fork");
	sem_unlink("sem_pause");
	sem_unlink("sem_kill_all");
	sem_unlink("sem_last_eat_time");
	sem_unlink("sem_eat_complete");
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->finished_philos = 0;
	data->nbr_philo_must_eat = -1;
	if (argv[5])
		data->nbr_philo_must_eat = ft_atoi(argv[5]);
	if (data->nbr_philo_must_eat == 0)
		return (1);
	return (0);
}
