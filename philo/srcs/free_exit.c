/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/06 20:11:05 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philo_list(philo_list *lst, int size)
{
	philo_list	*next;

	while (size)
	{
		next = lst->next;
		free(lst);
		lst = next;
		size--;
	}
}

int	check_error(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments.\n"), 1);
	i = 1;
	while (argv[i])
	{
		if (is_not_numeric(argv[i]))
			return (printf("Arguments must contain only numerics values, be positive and inferior to 2147483648.\n"), 1);
		if (is_more_10_char(argv[i]))
			return (printf("Arguments must contain only numerics values, be positive and inferior to 2147483648.\n"), 1);
		if (ft_atoi(argv[i]) < 0 || ft_atoi(argv[i]) > 2147483647)
			return (printf("Arguments must contain only numerics values, be positive and inferior to 2147483648.\n"), 1);
		i++;
	}
}
