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
	pthread_t		pthread_exit_check;
	int				eating_counter;

	gettimeofday(&ct, NULL);
	philo_data.data = &data;
	philo_data.index = index;
	philo_data.exit = 0;
	philo_data.last_eat_time = ((ct.tv_sec * 1000000 + ct.tv_usec) / 1000);
	eating_counter = 0;
	pthread_create(&pthread_exit_check, NULL, thread_exit_check, &philo_data);
	pthread_detach(pthread_exit_check);
	if (philo_data.index % 2 == 1)
		usleep((data.time_to_eat / 2) * 1000);
	while (1)
		routine(&philo_data, &eating_counter);
}

int	create_childs(pid_t *pid, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philos_nbr)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			child_func(data, i + 1);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	philos_eat_check;

	data.pid = malloc(data.philos_nbr * sizeof(pid_t));
	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	create_childs(data.pid, data);
	pthread_create(&philos_eat_check, NULL, philos_eat_check_func, &data);
	sem_wait(data.sem_kill_all);
	kill_all(data);
	pthread_join(philos_eat_check, NULL);
	free_and_exit(data);
}
