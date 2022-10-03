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
		printf("%ld %d has taken a fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
		pthread_mutex_lock(&lock);
		lst->previous->right_fork = 0;
		lst->left_fork = 0;
		pthread_mutex_unlock(&lock);
		return 1;
	}
	return 0;
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
		printf("%ld %d has taken a fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
		pthread_mutex_lock(&lock);
		lst->next->left_fork = 0;
		lst->right_fork = 0;
		pthread_mutex_unlock(&lock);
		return 1;
	}
}

void routine(struct philo_data *philo)
{
	int left_fork;
	int right_fork;
	struct timeval current_time;
	long time;

	if (!philo->is_dead)
		left_fork = take_left_fork(philo->fork_table, philo->index);
	if (!philo->is_dead)
		right_fork = take_right_fork(philo->fork_table, philo->index);
	if (left_fork == 1 && right_fork == 1)
	{
		gettimeofday(&current_time, NULL);
		if (!philo->is_dead)
			printf("%ld %d is eating\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);
		(*philo).last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		usleep((*philo).data->time_to_eat * 1000);
		gettimeofday(&current_time, NULL);
		if (!philo->is_dead)
			printf("%ld %d is sleeping\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);
		drop_left_fork(philo->fork_table, philo->index);
		drop_right_fork(philo->fork_table, philo->index);
		usleep((*philo).data->time_to_sleep * 1000);
		gettimeofday(&current_time, NULL);
		if (!philo->is_dead)
			printf("%ld %d is thinking\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);
	}
}

void	*philo_thread_func(void *p)
{
	struct philo_data *philo = (struct philo_data *)p;
	int i;
	int j;
	if ((*philo).data->number_of_times_each_philosopher_must_eat == 0)
		i = 1;
	else
		i = (*philo).data->number_of_times_each_philosopher_must_eat;
	if ((*philo).index % 2 == 1)
		usleep(100000);
	while (i)
	{
		routine(philo);
		if ((*philo).data->number_of_times_each_philosopher_must_eat)
			i--;
	}
	return (NULL);
}

void	check_philo_death(struct philo_data *philo)
{
	struct timeval current_time;
	long die_timestamp;
	long current_timestamp;
	int i;
	
	i = 0;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		while (i < (philo[0]).data->number_of_philosophers)
		{
			die_timestamp = ((philo[i]).last_eat_time + (philo[0]).data->time_to_die);
			current_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
			if (die_timestamp < current_timestamp)
			{
				(philo[i]).is_dead = 1;
				printf("%ld %d died\n", current_timestamp, (philo[i]).index);
				//return ;
				exit(0);
			}
			i++;
		}
		usleep(1000);
		i = 0;
	}
}

void	get_data(t_data *data, char **argv)
{
	(*data).number_of_philosophers = atoi(argv[1]);
	(*data).time_to_die = atoi(argv[2]);
	(*data).time_to_eat = atoi(argv[3]);
	(*data).time_to_sleep = atoi(argv[4]);
	if (argv[5])
	 	(*data).number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	else
		(*data).number_of_times_each_philosopher_must_eat = 0;
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
	
	if (argc != 5 && argc != 6)
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
		philo_data[i - 1].is_dead = 0;
		philo_data[i - 1].last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		pthread_create(&philo_thread[i - 1], NULL, philo_thread_func, &philo_data[i - 1]);
		i++;
	}
	check_philo_death(philo_data);
	i = 1;
	while (i <= data.number_of_philosophers)
	{
		pthread_join(philo_thread[i - 1], NULL);
		i++;
	}
	//-----------------------------------------//
	//				EXIT AND FREE				//
	//-----------------------------------------//
	free_philo_list(fork_table, data.number_of_philosophers);
	free(philo_data);
	pthread_mutex_destroy(&lock);
}