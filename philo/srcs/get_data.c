/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 16:11:25 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_data(t_data *data, char **argv)
{
	data->philos_nbr = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->is_dead = 0;
	data->finished_philos = 0;
	data->nbr_philo_must_eat = -1;
	if (argv[5])
		data->nbr_philo_must_eat = ft_atoi(argv[5]);
	if (data->nbr_philo_must_eat == 0)
		return (1);
	if (pthread_mutex_init(&data->lock_fork, NULL) != 0)
		return (printf("Mutex init failed\n"), 1);
	if (pthread_mutex_init(&data->lock_eat, NULL) != 0)
		return (pthread_mutex_destroy(&data->lock_fork),
			printf("Mutex init failed\n"), 1);
	if (pthread_mutex_init(&data->lock_dead, NULL) != 0)
		return (pthread_mutex_destroy(&data->lock_fork),
			pthread_mutex_destroy(&data->lock_eat),
			printf("Mutex init failed\n"), 1);
	return (0);
}

void	get_table(t_philo_list **fork_table, int philos_nbr)
{
	int	i;

	*fork_table = NULL;
	i = 1;
	while (i <= philos_nbr)
		ft_lstadd_back(fork_table, ft_lstnew(i++));
	ft_lstset_previous(*fork_table);
	set_circular(*fork_table);
}
