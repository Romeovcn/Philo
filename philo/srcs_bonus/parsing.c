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
	return (0);
}