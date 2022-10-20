/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/19 15:40:36 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	close_sems(t_data data)
{
	if (data.sem_pause != SEM_FAILED)
		sem_close(data.sem_pause);
	if (data.sem_fork != SEM_FAILED)
		sem_close(data.sem_fork);
	if (data.sem_kill_all != SEM_FAILED)
		sem_close(data.sem_kill_all);
	if (data.sem_last_eat_time != SEM_FAILED)
		sem_close(data.sem_last_eat_time);
	if (data.sem_eat_complete != SEM_FAILED)
		sem_close(data.sem_eat_complete);
	sem_unlink("sem_pause");
	sem_unlink("sem_fork");
	sem_unlink("sem_kill_all");
	sem_unlink("sem_last_eat_time");
	sem_unlink("sem_eat_complete");
}

void	kill_all(t_data data)
{
	int	i;

	i = 0;
	while (i < data.philos_nbr)
		kill(data.pid[i++], SIGKILL);
	i = 0;
	while (i++ < data.philos_nbr)
		sem_post(data.sem_eat_complete);
}

void	fork_fail_exit(t_data data, int i)
{
	i--;
	while (i >= 0)
		kill(data.pid[i--], SIGKILL);
	close_sems(data);
	free(data.pid);
	exit (1);
}
