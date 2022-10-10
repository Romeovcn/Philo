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

void go_to_sleep(long timestamp_to_wait)
{
	struct timeval current_time;

	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000) >= timestamp_to_wait)
			return ;
		usleep(1000);
	}
	return ;
}

int	check_dead(t_data *data)
{
	pthread_mutex_lock(&data->lock_dead);
    int result = data->is_dead;
 	pthread_mutex_unlock(&data->lock_dead);
    return result;
}

int print_action(struct philo_data *philo, char *message, struct timeval current_time)
{
	pthread_mutex_lock(&philo->data->lock_dead);
	if (!philo->data->is_dead)
		printf("%ld %d %s", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index, message);
	else
	{
		pthread_mutex_unlock(&philo->data->lock_dead);
		return 1;
	}
	pthread_mutex_unlock(&philo->data->lock_dead);
}

int routine(struct philo_data *philo)
{
	int left_fork;
	int right_fork;
	struct timeval current_time;

	left_fork = take_left_fork(philo->fork_table, philo->index, philo);
	right_fork = take_right_fork(philo->fork_table, philo->index, philo);
	if (left_fork && right_fork)
	{
		gettimeofday(&current_time, NULL);
		print_action(philo, "is eating\n", current_time);
		pthread_mutex_lock(&philo->data->lock_eat);
		philo->last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		pthread_mutex_unlock(&philo->data->lock_eat);
		usleep((*philo).data->time_to_eat * 1000);
		gettimeofday(&current_time, NULL);
		print_action(philo, "is sleeping\n", current_time);
		pthread_mutex_lock(&philo->data->lock_fork);
		drop_left_fork(philo->fork_table, philo->index);
		drop_right_fork(philo->fork_table, philo->index);
		pthread_mutex_unlock(&philo->data->lock_fork);
		usleep((*philo).data->time_to_sleep * 1000);
		gettimeofday(&current_time, NULL);
		print_action(philo, "is thinking\n", current_time);
	}
	if (left_fork == -1 || right_fork == -1)
		return 1;
	return 0;
}

void	*philo_thread_func(void *p)
{
	struct philo_data *philo;
	int i;
	int j;

	philo = (struct philo_data *)p;
	if ((*philo).index % 2 == 1)
		usleep(10000);
	while (1)
	{
		if (routine(philo))
			break ;
	}
	// philo->data->finished_philos++;
	// printf("FINISHED=%d\n", philo->data->finished_philos);
	return NULL;
}

void check_philo_death(struct philo_data *philo, pthread_t *philo_thread)
{
	struct timeval current_time;
	long die_timestamp;
	long current_timestamp;
	int i;

	while (1)
	{
		i = 0;
		gettimeofday(&current_time, NULL);
		while (i < philo->data->number_of_philosophers)
		{
			pthread_mutex_lock(&philo->data->lock_eat);
			die_timestamp = (philo[i].last_eat_time + philo->data->time_to_die);
			pthread_mutex_unlock(&philo->data->lock_eat);
			current_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
			if (die_timestamp < current_timestamp)
			{
				pthread_mutex_lock(&philo->data->lock_dead);
				philo->data->is_dead = 1;
				pthread_mutex_unlock(&philo->data->lock_dead);
				printf("%ld %d died\n", die_timestamp, (philo[i]).index);
				return ;
			}
			i++;
		}
		// usleep(1000);
	}
}

int	get_data(t_data *data, char **argv)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->is_dead = 0;
	data->finished_philos = 0;
	if (argv[5])
	 	data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = 0;
	if (pthread_mutex_init(&data->lock_fork, NULL) != 0)
        return (printf("Mutex init failed\n"), 1);
	if (pthread_mutex_init(&data->lock_eat, NULL) != 0)
        return (printf("Mutex init failed\n"), 1);
	if (pthread_mutex_init(&data->lock_dead, NULL) != 0)
        return (printf("Mutex init failed\n"), 1);
}

int init_threads(t_data *data, philo_list *fork_table)
{
	pthread_t *philo_thread;
	struct philo_data *philo_data;
	struct timeval current_time;
	int i;

	philo_thread = malloc(data->number_of_philosophers * sizeof(pthread_t));
	philo_data = malloc(data->number_of_philosophers * sizeof(struct philo_data));
	i = 1;
	gettimeofday(&current_time, NULL);
	data->start_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
	while (i <= data->number_of_philosophers)
	{
		philo_data[i - 1].fork_table = fork_table;
		philo_data[i - 1].data = data;
		philo_data[i - 1].index = i;
		philo_data[i - 1].last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		pthread_create(&philo_thread[i - 1], NULL, philo_thread_func, &philo_data[i - 1]);
		i++;
	}
	check_philo_death(philo_data, philo_thread);
	i = 1;
	while (i <= data->number_of_philosophers)
	{
		pthread_join(philo_thread[i - 1], NULL);
		i++;
	}
	free(philo_thread);
	free(philo_data);
	return 0;
}

int	main(int argc, char **argv)
{
	t_data data;
	int i;
	philo_list *fork_table;
	//-----------------------------------------//
	//				Fork list				   //
	//-----------------------------------------//
	if (check_error(argc, argv))
		return 1;
	get_data(&data, argv);
	fork_table = NULL;
	i = 1;
	while (i <= data.number_of_philosophers)
		ft_lstadd_back(&fork_table, ft_lstnew(i++));
	ft_lstset_previous(fork_table);
	set_circular(fork_table);
	//------------------------------------------//
	//					Thread					//
	//------------------------------------------//
	init_threads(&data, fork_table);
	//------------------------------------------//
	//				EXIT AND FREE				//
	//------------------------------------------//
	free_philo_list(fork_table, data.number_of_philosophers);
	pthread_mutex_destroy(&data.lock_eat);
	pthread_mutex_destroy(&data.lock_fork);
	pthread_mutex_destroy(&data.lock_dead);
}