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

void	free_and_exit(t_data data)
{
	free(data.pid);
	sem_close(data.sem_pause);
	sem_close(data.sem_fork);
	sem_close(data.sem_kill_all);
	sem_close(data.sem_last_eat_time);
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
	while (i < data.number_of_philosophers)
		kill(data.pid[i++], SIGKILL);
	i = 0;
	while (i++ < data.number_of_philosophers)
		sem_post(data.sem_eat_complete);
}
