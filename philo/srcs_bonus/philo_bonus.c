/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 19:11:18 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	child_func(t_data data, int index)
{
	struct timeval	ct;
	t_philo_data	philo_data;
	pthread_t		death_check;
	int				eating_counter;

	gettimeofday(&ct, NULL);
	philo_data.data = &data;
	philo_data.index = index;
	philo_data.last_eat_time = ((ct.tv_sec * 1000000 + ct.tv_usec) / 1000);
	eating_counter = 0;
	if (pthread_create(&death_check, NULL, death_check_func, &philo_data) == 0)
		pthread_detach(death_check);
	if (philo_data.index % 2 == 1)
		usleep((data.time_to_eat / 2) * 1000);
	while (1)
		routine(&philo_data, &eating_counter);
}

int	create_childs(t_data *data)
{
	struct timeval	ct;
	int				i;

	i = 0;
	gettimeofday(&ct, NULL);
	data->start_timestamp = (ct.tv_sec * 1000000 + ct.tv_usec) / 1000;
	while (i < data->philos_nbr)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			fork_fail_exit(*data, i);
		if (data->pid[i] == 0)
			child_func(*data, i + 1);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t	philos_eat_check;
	t_data		data;

	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	data.pid = malloc(data.philos_nbr * sizeof(pid_t));
	if (!data.pid)
		return (close_sems(data), 1);
	create_childs(&data);
	if (pthread_create(&philos_eat_check, NULL, philos_eat_check_func, &data))
		return (kill_all(data), close_sems(data), free(data.pid), 1);
	sem_wait(data.sem_kill_all);
	kill_all(data);
	pthread_join(philos_eat_check, NULL);
	close_sems(data);
	free(data.pid);
}
