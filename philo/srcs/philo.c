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

void	join_threads(pthread_t *philo_thread, int philos_nbr)
{
	int	i;

	i = 1;
	while (i <= philos_nbr)
	{
		pthread_join(philo_thread[i - 1], NULL);
		i++;
	}
}

void	*philo_thread_func(void *p)
{
	t_philo_data	*philo;

	philo = (t_philo_data *)p;
	if ((*philo).index % 2 == 1)
		usleep(10000);
	while (1)
		if (philo_routine(philo))
			break ;
	return (NULL);
}

int	init_threads(t_data *data, t_philo_list *fork_table,
		pthread_t *philo_thread, t_philo_data *philo_data)
{
	struct timeval	ct;
	int				i;

	i = 0;
	gettimeofday(&ct, NULL);
	data->start_timestamp = get_time_stamp(ct);
	while (++i <= data->philos_nbr)
	{
		philo_data[i - 1].fork_table = fork_table;
		philo_data[i - 1].data = data;
		philo_data[i - 1].index = i;
		philo_data[i - 1].nbr_of_eat = 0;
		philo_data[i - 1].last_eat_time = get_time_stamp(ct);
		if (pthread_create(&philo_thread[i - 1], NULL, philo_thread_func,
				&philo_data[i - 1]) != 0)
		{
			pthread_mutex_lock(&data->lock_dead);
			data->is_dead = 1;
			pthread_mutex_unlock(&data->lock_dead);
			thread_error_join(philo_thread, i);
			return (printf("Error while initializing philo %d\n", i), 1);
		}
	}
	return (0);
}

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
	if (get_table(&fork_table, data.philos_nbr))
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	philo_thread = malloc(data.philos_nbr * sizeof(pthread_t));
	if (!philo_thread)
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	philo_data = malloc(data.philos_nbr * sizeof(t_philo_data));
	if (!philo_data)
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	if (init_threads(&data, fork_table, philo_thread, philo_data))
		return (free_and_exit(data, fork_table, philo_thread, philo_data), 1);
	check_philo_death(philo_data);
	join_threads(philo_thread, data.philos_nbr);
	free_and_exit(data, fork_table, philo_thread, philo_data);
}
