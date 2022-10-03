#include "philo.h"

pthread_mutex_t lock_f;

pthread_mutex_t lock_d;

pthread_mutex_t lock_eat;

int	check_dead(int is_d)
{
	pthread_mutex_lock(&lock_d);
    int result = is_d;
 	pthread_mutex_unlock(&lock_d);
    return result;
}

int take_left_fork(philo_list *lst, int index, p_data *philo)
{
	struct timeval current_time;

	while (lst->index != index)
		lst = lst->next;
	while (1)
	{
		pthread_mutex_lock(&lock_f);
		if (lst->left_fork && !check_dead(philo->is_dead))
		{
			gettimeofday(&current_time, NULL);
			printf("%ld %d has taken a left fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
			lst->previous->right_fork = 0;
			lst->left_fork = 0;
			pthread_mutex_unlock(&lock_f);
			return 1;
		}
		pthread_mutex_unlock(&lock_f);
		usleep(1000);
	}
	return 0;
}

int take_right_fork(philo_list *lst, int index, p_data *philo)
{
	struct timeval current_time;

	while (lst->index != index)
		lst = lst->next;
	while (1)
	{
			pthread_mutex_lock(&lock_f);
		if (lst->right_fork && !check_dead(philo->is_dead))
		{
			gettimeofday(&current_time, NULL);
			printf("%ld %d has taken a right fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
			lst->next->left_fork = 0;
			lst->right_fork = 0;
			pthread_mutex_unlock(&lock_f);
			return 1;
		}
		pthread_mutex_unlock(&lock_f);
		usleep(1000);
	}
	return 0;
}

void routine(struct philo_data *philo)
{
	int left_fork;
	int right_fork;
	struct timeval current_time;
	long time;

	left_fork = take_left_fork(philo->fork_table, philo->index, philo);
	right_fork = take_right_fork(philo->fork_table, philo->index, philo);
	if (left_fork == 1 && right_fork == 1)
	{
		gettimeofday(&current_time, NULL);

		if (!check_dead(philo->is_dead))
			printf("%ld %d is eating\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);

		pthread_mutex_lock(&lock_eat);
		philo->last_eat_time = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
		pthread_mutex_unlock(&lock_eat);

		usleep((*philo).data->time_to_eat * 1000);

		gettimeofday(&current_time, NULL);

		if (!check_dead(philo->is_dead))
		{
			printf("%ld %d is sleeping\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), philo->index);
		}

		pthread_mutex_lock(&lock_f);
		drop_left_fork(philo->fork_table, philo->index);
		drop_right_fork(philo->fork_table, philo->index);
		pthread_mutex_unlock(&lock_f);

		usleep((*philo).data->time_to_sleep * 1000);
		gettimeofday(&current_time, NULL);

		if (!check_dead(philo->is_dead))
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
		{
			printf("KGFHJHGDFKJGHDFJKGH\n");
			i--;
		}
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
		while (i < philo->data->number_of_philosophers)
		{
			pthread_mutex_lock(&lock_eat);
			die_timestamp = (philo[i].last_eat_time + philo->data->time_to_die);
			pthread_mutex_unlock(&lock_eat);
			current_timestamp = ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000);
			if (die_timestamp < current_timestamp)
			{
				pthread_mutex_lock(&lock_d);
				philo[i].is_dead = 1;
				printf("%ld %d died\n", current_timestamp, (philo[i]).index);
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
	if (pthread_mutex_init(&lock_f, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
	if (pthread_mutex_init(&lock_d, NULL) != 0)
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
	pthread_mutex_destroy(&lock_eat);
	pthread_mutex_destroy(&lock_f);
	pthread_mutex_destroy(&lock_d);
}