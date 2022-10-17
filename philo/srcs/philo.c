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
	t_philo_list		*fork_table;
	pthread_t			*philo_thread;
	struct philo_data	*philo_data;

	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	get_table(&fork_table, data.number_of_philosophers);
	philo_thread = malloc(data.number_of_philosophers * sizeof(pthread_t));
	philo_data = malloc(data.number_of_philosophers
			* sizeof(struct philo_data));
	init_threads(&data, fork_table, philo_thread, philo_data);
	check_philo_death(philo_data);
	join_threads(philo_thread, data.number_of_philosophers);
	free_and_exit(data, fork_table, philo_thread, philo_data);
}
