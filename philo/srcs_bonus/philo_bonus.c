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
	struct timeval	current_time;
	t_philo_data	philo_data;
	pthread_t		pthread_exit_check;
	int				eating_counter;

	gettimeofday(&current_time, NULL);
	philo_data.data = &data;
	philo_data.index = index + 1;
	philo_data.exit = 0;
	philo_data.last_eat_time = ((current_time.tv_sec * 1000000
				+ current_time.tv_usec) / 1000);
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
	while (i < data.number_of_philosophers)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			child_func(data, i);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	pthread_check_finished_philos;

	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	data.sem_fork = sem_open("sem_fork", O_CREAT, 0666,
			data.number_of_philosophers);
	data.sem_pause = sem_open("sem_pause", O_CREAT, 0666, 1);
	data.sem_kill_all = sem_open("sem_kill_all", O_CREAT, 0666, 0);
	data.sem_last_eat_time = sem_open("sem_last_eat_time", O_CREAT, 0666, 1);
	data.sem_eat_complete = sem_open("sem_eat_complete", O_CREAT, 0666, 0);
	data.pid = malloc(data.number_of_philosophers * sizeof(pid_t));
	create_childs(data.pid, data);
	pthread_create(&pthread_check_finished_philos, NULL,
		thread_check_finished_philos, &data);
	sem_wait(data.sem_kill_all);
	printf("KIlling all...\n");
	kill_all(data);
	pthread_join(pthread_check_finished_philos, NULL);
	free_and_exit(data);
}
