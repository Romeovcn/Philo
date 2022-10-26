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
	data->philos_nbr = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nbr_philo_must_eat = -1;
	if (argv[5])
		data->nbr_philo_must_eat = ft_atoi(argv[5]);
	if (data->nbr_philo_must_eat == 0)
		return (1);
	sem_unlink("sem_fork");
	sem_unlink("sem_pause");
	sem_unlink("sem_kill_all");
	sem_unlink("sem_last_eat_time");
	sem_unlink("sem_eat_complete");
	data->sem_fork = sem_open("sem_fork", O_CREAT, 0666, data->philos_nbr);
	data->sem_pause = sem_open("sem_pause", O_CREAT, 0666, 1);
	data->sem_kill_all = sem_open("sem_kill_all", O_CREAT, 0666, 0);
	data->sem_last_eat_time = sem_open("sem_last_eat_time", O_CREAT, 0666, 1);
	data->sem_eat_complete = sem_open("sem_eat_complete", O_CREAT, 0666, 0);
	if (data->sem_fork == SEM_FAILED || data->sem_pause == SEM_FAILED
		|| data->sem_kill_all == SEM_FAILED
		|| data->sem_eat_complete == SEM_FAILED
		|| data->sem_last_eat_time == SEM_FAILED)
		return (close_sems(*data), 1);
	return (0);
}
