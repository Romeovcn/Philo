#include "philo.h"

void drop_left_fork(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	//pthread_mutex_lock(&lock);
	lst->previous->right_fork = 1;
	lst->left_fork = 1;
	//pthread_mutex_unlock(&lock);
}

void drop_right_fork(philo_list *lst, int index)
{
	while (lst->index != index)
		lst = lst->next;
	//pthread_mutex_lock(&lock);
	lst->previous->left_fork = 1;
	lst->right_fork = 1;
	//pthread_mutex_unlock(&lock);
}