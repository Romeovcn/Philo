#include "philo.h"

philo_list	*ft_lstnew(int index)
{
	philo_list	*result;

	result = malloc(sizeof(philo_list));
	if (!result)
		return (0);
	result->index = index;
	result->left = 1;
	result->right = 1;
	result->next = NULL;
	result->previous = NULL;
	return (result);
}

philo_list	*ft_lstlast(philo_list *lst)
{
	philo_list	*head;

	if (lst == NULL)
		return (0);
	head = lst;
	while (head->next)
		head = head->next;
	return (head);
}

void	ft_lstadd_back(philo_list **lst, philo_list *new)
{
	philo_list	*last;

	if (*lst)
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
	else
		*lst = new;
}

void	ft_lstset_previous(philo_list *lst)
{
	while (lst->next)
	{
		lst->next->previous = lst;
		lst = lst->next;
	}
}

void	set_circular(philo_list *lst)
{
	philo_list	*head;

	head = lst;
	while (lst->next)
		lst = lst->next;
	head->previous = lst;
	lst->next = head;
}