#include "philo.h"

void drop_left_fork(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	lst->previous->right_fork = 1;
	lst->left_fork = 1;
}

void drop_right_fork(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	lst->previous->left_fork = 1;
	lst->right_fork = 1;
}

int take_left_fork(philo_list *lst, int index, p_data *philo)
{
	struct timeval current_time;

	while (lst->index != index)
		lst = lst->next;
	while (1)
	{
		if (check_dead(philo->data))
			return (-1);
		else
		{
			pthread_mutex_lock(&philo->data->lock_fork);
			if (lst->left_fork)
			{
				gettimeofday(&current_time, NULL);
				printf("%ld %d has taken a fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
				lst->previous->right_fork = 0;
				lst->left_fork = 0;
				pthread_mutex_unlock(&philo->data->lock_fork);
				return 1;
			}
			pthread_mutex_unlock(&philo->data->lock_fork);
		}
		// usleep(1000);
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
		if (check_dead(philo->data))
			return -1;
		else
		{
			pthread_mutex_lock(&philo->data->lock_fork);
			if (lst->right_fork)
			{
				gettimeofday(&current_time, NULL);
				printf("%ld %d has taken a fork\n", ((current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000), index);
				lst->next->left_fork = 0;
				lst->right_fork = 0;
				pthread_mutex_unlock(&philo->data->lock_fork);
				return 1;
			}
			pthread_mutex_unlock(&philo->data->lock_fork);
		}
		// usleep(1000);
	}
	return 0;
}