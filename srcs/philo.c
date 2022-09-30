#include "philo.h"

pthread_mutex_t lock;

int take_left_fork(philo_list *lst, int index)
{
	struct timeval current_time;
	while (lst->index != index)
		lst = lst->next;
	while (!lst->left_fork)
	{
		if (lst->left_fork)
			break;
	}
	if (lst->left_fork)
	{
		gettimeofday(&current_time, NULL);
		printf("%ld %ld %d has taken left fork\n", current_time.tv_sec, current_time.tv_usec, index);
		pthread_mutex_lock(&lock);
		lst->previous->right_fork = 0;
		lst->left_fork = 0;
		pthread_mutex_unlock(&lock);
		return 1;
	}
}

int take_right_fork(philo_list *lst, int index)
{
	struct timeval current_time;
	while (lst->index != index)
		lst = lst->next;
	while (!lst->right_fork)
	{
		if (lst->right_fork)
			break;
	}
	if (lst->right_fork)
	{	
		gettimeofday(&current_time, NULL);
		printf("%ld %ld %d has taken right fork\n", current_time.tv_sec, current_time.tv_usec, index);
		pthread_mutex_lock(&lock);
		lst->next->left_fork = 0;
		lst->right_fork = 0;
		pthread_mutex_unlock(&lock);
		return 1;
	}
}

int	check_can_eat(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	if (lst->left_fork && lst->right_fork)
		return (1);
	return (0);
}

void	set_eat(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	lst->left_fork = 0;
	lst->right_fork = 0;
	lst->previous->right_fork = 0;
	lst->next->left_fork = 0;
}

void routine(struct philo_data *philo)
{
	int left_fork;
	int right_fork;
	struct timeval current_time;
	long time;

	left_fork = take_left_fork(philo->fork_table, philo->index);
	right_fork = take_right_fork(philo->fork_table, philo->index);
	if (left_fork == 1 && right_fork == 1)
	{
		gettimeofday(&current_time, NULL);
		printf("%ld %ld %d is eating\n", current_time.tv_sec, current_time.tv_usec, philo->index);
		(*philo).last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		sleep(3);
		gettimeofday(&current_time, NULL);
		printf("%ld %ld %d is sleeping\n", current_time.tv_sec, current_time.tv_usec, philo->index);
		pthread_mutex_lock(&lock);
		drop_left_fork(philo->fork_table, philo->index);
		drop_right_fork(philo->fork_table, philo->index);
		pthread_mutex_unlock(&lock);
		sleep(3);
		gettimeofday(&current_time, NULL);
		printf("%ld %ld %d is thinking\n", current_time.tv_sec, current_time.tv_usec, philo->index);
		// printf("%ld %ld %d droped is forks\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);
	}
}

void	*philo_thread_func(void *p)
{
	struct philo_data *philo = (struct philo_data *)p;
	if ((*philo).index % 2 == 1)
		usleep(100000);
	while (1)
	{
		routine(philo);
	}

	return (NULL);
}

void	*philo_death_thread_func(void *p)
{
	struct philo_data *philo = (struct philo_data *)p;
	long time_to_die = (philo[0]).data->time_to_die;
	struct timeval current_time;
	int i = 0;

	while (1)
	{
		gettimeofday(&current_time, NULL);
		while (i < (philo[0]).data->number_of_philosophers)
		{
			long hour_to_die = ((philo[i]).last_eat_time + time_to_die);
			long current_hour = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
			if (hour_to_die < current_hour)
			{
				printf("%ld <= %ld\n", hour_to_die, current_hour);
				printf("%ld %d died ; last eat = %ld\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), (philo[i]).index, (philo[i]).last_eat_time);
				// exit(0);
			}
			i++;
		}
		i = 0;
	}
	return (NULL);
}

void	get_data(t_data *data, char **argv)
{
	(*data).number_of_philosophers = atoi(argv[1]);
	(*data).time_to_die = atoi(argv[2]);
	// (*data).time_to_eat = atoi(argv[3]);
	// (*data).time_to_sleep = atoi(argv[4]);
	// if (argv[5])
	// 	(*data).number_of_times_each_philosopher_must_eat = atoi(argv[5]);
}

void free_philo_list(philo_list *lst, int size)
{
	philo_list *next;
	while (size)
	{
		next = lst->next;
		free(lst);
		lst = next;
		size--;
	}
}

int	main(int argc, char **argv)
{
	pthread_t *philo_thread;
	pthread_t philo_death_thread;
	struct philo_data *philo_data;
	t_data data;
	int i;
	struct timeval current_time;
	
	if (argc != 3 && argc != 5)
	{
		printf("Wrong inputs\n");
		exit(1);
	}
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
	//-----------------------------------------//
	//				Fork list				   //
	//-----------------------------------------//
	get_data(&data, argv);

	philo_list *fork_table;

	fork_table = NULL;
	i = 1;
	while (i <= data.number_of_philosophers)
		ft_lstadd_back(&fork_table, ft_lstnew(i++));
	ft_lstset_previous(fork_table);
	set_circular(fork_table);
	//-----------------------------------------//
	//					Thread					//
	//-----------------------------------------//
	philo_thread = malloc(data.number_of_philosophers * sizeof(pthread_t));
	philo_data = malloc(data.number_of_philosophers * sizeof(struct philo_data));
	i = 1;
	gettimeofday(&current_time, NULL);
	while (i <= data.number_of_philosophers)
	{
		philo_data[i - 1].fork_table = fork_table;
		philo_data[i - 1].data = &data;
		philo_data[i - 1].index = i;
		philo_data[i - 1].last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		pthread_create(&philo_thread[i - 1], NULL, philo_thread_func, &philo_data[i - 1]);
		i++;
	}
	// pthread_create(&philo_death_thread, NULL, philo_death_thread_func, philo_data);
	i = 1;
	while (i <= data.number_of_philosophers)
	{
		pthread_join(philo_thread[i - 1], NULL);
		i++;
	}
	// pthread_join(philo_death_thread, NULL);
	//-----------------------------------------//
	//				EXIT AND FREE				//
	//-----------------------------------------//
	free_philo_list(fork_table, data.number_of_philosophers);
	free(philo_data);
	pthread_mutex_destroy(&lock);
	// struct timeval current_time;
	// long long total;
	// while (1)
	// {
	// 	gettimeofday(&current_time, NULL);
	// 	total = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
	// 	// printf("%ld %ld\n", current_time.tv_sec, current_time.tv_usec);
	// 	printf("%ld\n", total);
	// 	if (current_time.tv_usec < 20)
	// 	{
	// 		gettimeofday(&current_time, NULL);
	// 		total = (current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000;
	// 		printf("%ld\n", total);
	// 		printf("%ld %ld\n", current_time.tv_sec, current_time.tv_usec);
	// 		break ;
	// 	}

	// }
}