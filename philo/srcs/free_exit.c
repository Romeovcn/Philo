/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/06 20:11:05 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philo_list(t_philo_list *lst, int size)
{
	t_philo_list	*next;

	while (lst && size)
	{
		next = lst->next;
		free(lst);
		lst = next;
		size--;
	}
}

void	free_and_exit(t_data data, t_philo_list *fork_table,
		pthread_t *philo_thread, t_philo_data *philo_data)
{
	free(philo_thread);
	free(philo_data);
	free_philo_list(fork_table, data.philos_nbr);
	pthread_mutex_destroy(&data.lock_eat);
	pthread_mutex_destroy(&data.lock_fork);
	pthread_mutex_destroy(&data.lock_dead);
}

void	thread_error_join(pthread_t *philo_thread, int i)
{
	int	j;

	j = 1;
	while (j < i)
	{
		pthread_join(philo_thread[j - 1], NULL);
		j++;
	}
}
