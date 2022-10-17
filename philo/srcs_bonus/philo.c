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

void kill_all(t_data data)
{
	int i;

	i = 0;
	while (i < data.number_of_philosophers)
	{
		kill(data.pid[i++], SIGKILL);
	}
}

// void	go_to_sleep(long timestamp_to_wait, t_data *data)
// {
// 	struct timeval	current_time;

// 	while (1)
// 	{
// 		gettimeofday(&current_time, NULL);
// 		if (check_dead(data))
// 			return ;
// 		if (((current_time.tv_sec * 1000000 + current_time.tv_usec)
// 				/ 1000) >= timestamp_to_wait)
// 			return ;
// 		usleep(100);
// 	}
// 	return ;
// }

int post_message(t_philo_data	philo_data, char *message, long time_to_wait, struct timeval current_time)
{
	if (philo_data.exit)
		return (1);
	sem_wait(philo_data.data->sem_death);
	printf("%ld %d %s\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo_data.index, message);
	sem_post(philo_data.data->sem_death);
	usleep(time_to_wait);
	return 0;
}

void	*thread_check_death(void *p)
{
	t_philo_data	*philo_data;
	struct timeval current_time;
	long	die_timestamp;
	long	current_timestamp;

	philo_data = (t_philo_data *)p;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		sem_wait(philo_data->data->sem_last_eat_time);
		die_timestamp = (philo_data->last_eat_time + philo_data->data->time_to_die);
		sem_post(philo_data->data->sem_last_eat_time);
		current_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		sem_wait(philo_data->data->sem_exit);
		if (die_timestamp < current_timestamp)
		{
			philo_data->exit = 1;
			sem_wait(philo_data->data->sem_death);
			printf("%d DIED-------------------------\n", philo_data->index);
			sem_post(philo_data->data->sem_fork);
			return NULL;
		}
		sem_post(philo_data->data->sem_exit);
		usleep(1000);
	}
	return NULL;
}

int take_fork(t_philo_data	*philo_data)
{
	struct timeval	current_time;

	if (philo_data->exit)
		return (1);
	else
	{
		sem_wait(philo_data->data->sem_fork);
		if (philo_data->exit)
			return (1);
		sem_wait(philo_data->data->sem_death);
		gettimeofday(&current_time, NULL);
		printf("%ld %d take fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo_data->index);
		sem_post(philo_data->data->sem_death);
		return 0;
	}
	// data race bc of exit, put sem on philo_data->exit variable
}

int child_func(t_data data, int index)
{
	struct timeval	current_time;
	t_philo_data	philo_data;
	pthread_t pthread_check_death;
	pthread_t pthread_check_exit;

	gettimeofday(&current_time, NULL);
	philo_data.data = &data;
	philo_data.index = index + 1;
	philo_data.exit = 0;
	philo_data.last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
	pthread_create(&pthread_check_death, NULL, thread_check_death, &philo_data);
	if (index % 2 == 1)
		usleep(10000);
	while (1)
	{
		if (take_fork(&philo_data))
			break ;
		if (take_fork(&philo_data))
			break ;
		gettimeofday(&current_time, NULL);
		sem_wait(philo_data.data->sem_last_eat_time);
		philo_data.last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		sem_post(philo_data.data->sem_last_eat_time);
		if (post_message(philo_data, "is eating", data.time_to_eat * 1000, current_time))
			break ;
		sem_post(data.sem_fork);
		sem_post(data.sem_fork);
		gettimeofday(&current_time, NULL);
		if (post_message(philo_data, "is sleeping", data.time_to_sleep * 1000, current_time))
			break ;
		gettimeofday(&current_time, NULL);
		if (post_message(philo_data, "is thinking", 0, current_time))
			break;
	}
	pthread_join(pthread_check_death, NULL);
	free_and_exit(data);
	exit(1);
}

int create_childs(pid_t *pid, t_data data)
{
	int i;

	i = 0;
	while (i < data.number_of_philosophers)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			child_func(data, i);
		i++;
	}
	return 0;
}

int main(int argc, char **argv)
{
	t_data data;
	sem_unlink("sem_fork");
	sem_unlink("sem_death");
	sem_unlink("sem_exit");
	sem_unlink("sem_last_eat_time");
	sem_unlink("sem_exit_mutex");
	int i = 0;

	if (check_error(argc, argv) || get_data(&data, argv))
		return (1);
	data.sem_fork = sem_open("sem_fork", O_CREAT, 0666, data.number_of_philosophers);
	data.sem_death = sem_open("sem_death", O_CREAT, 0666, 1);
	data.sem_exit = sem_open("sem_exit", O_CREAT, 0666, 1);
	data.sem_last_eat_time = sem_open("sem_last_eat_time", O_CREAT, 0666, 1);
	data.sem_exit_mutex = sem_open("sem_exit_mutex", O_CREAT, 0666, 0);
	data.pid = malloc(data.number_of_philosophers * sizeof(pid_t));
	create_childs(data.pid, data);
	int status = 0;
	i = 0;
	while(waitpid(0, &status, 0) > 0)
	{
		if (WEXITSTATUS(status) == 1)
		{
			printf("Exit code 1 received\n");
			kill_all(data);
		}
		// printf("Parent received child %d with status %d\n", i + 1, WEXITSTATUS(status));
		i++;
	}
	free_and_exit(data);
}