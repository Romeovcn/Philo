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