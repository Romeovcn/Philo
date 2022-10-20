/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/10 18:50:19 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data				data;
	t_philo_data		*philo_data;
	t_philo_list		*fork_table;
	pthread_t			*philo_thread;

	philo_thread = NULL;
	philo_data = NULL;
	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	get_table(&fork_table, data.philos_nbr);
	philo_thread = malloc(data.philos_nbr * sizeof(pthread_t));
	if (!philo_thread)
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	philo_data = malloc(data.philos_nbr * sizeof(t_philo_data));
	if (!philo_data)
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	init_threads(&data, fork_table, philo_thread, philo_data);
	check_philo_death(philo_data);
	join_threads(philo_thread, data.philos_nbr);
	free_and_exit(data, fork_table, philo_thread, philo_data);
}
