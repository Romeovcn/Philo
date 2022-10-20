/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/06 19:30:53 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo_list	*ft_lstnew(int index)
{
	t_philo_list	*result;

	result = malloc(sizeof(t_philo_list));
	if (!result)
		return (NULL);
	result->index = index;
	result->left_fork = 1;
	result->right_fork = 1;
	result->next = NULL;
	result->previous = NULL;
	return (result);
}

t_philo_list	*ft_lstlast(t_philo_list *lst)
{
	t_philo_list	*head;

	if (lst == NULL)
		return (0);
	head = lst;
	while (head->next)
		head = head->next;
	return (head);
}

void	ft_lstadd_back(t_philo_list **lst, t_philo_list *new)
{
	t_philo_list	*last;

	if (*lst)
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
	else
		*lst = new;
}

void	ft_lstset_previous(t_philo_list *lst)
{
	while (lst->next)
	{
		lst->next->previous = lst;
		lst = lst->next;
	}
}

void	set_circular(t_philo_list *lst)
{
	t_philo_list	*head;

	head = lst;
	while (lst->next)
		lst = lst->next;
	head->previous = lst;
	lst->next = head;
}
