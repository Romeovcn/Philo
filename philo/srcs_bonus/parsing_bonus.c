/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 19:26:48 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	ft_atoi(const char *str)
{
	long	result;
	int		i;
	int		n;

	n = 0;
	result = 0;
	i = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i + 1] < '0' || str[i + 1] > '9')
			return (0);
		if (str[i] == '-')
			n = 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	if (n)
		result = result * -1;
	return (result);
}

int	is_not_numeric(char *s)
{
	while (*s)
	{
		if (*s > '9' || *s < '0')
			return (1);
		s++;
	}
	return (0);
}

int	is_more_10_char(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	if (i > 10)
		return (1);
	return (0);
}

int	check_error(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments.\n"), 1);
	if (atoi(argv[1]) == 0)
		return (printf("Number of philosophers must be greater than 0.\n"), 1);
	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 0)
			return (printf("Arguments must be positive.\n"), 1);
		if (is_not_numeric(argv[i]))
			return (printf("Arguments must contains only numerics values.\n"),
				1);
		if (is_more_10_char(argv[i]) || ft_atoi(argv[i]) > 2147483647)
			return (printf("Arguments must be smaller than 2147483648.\n"), 1);
		i++;
	}
	return (0);
}
