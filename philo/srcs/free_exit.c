#include "philo.h"

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