#include "philo_bonus.h"

int	get_data(t_data *data, char **argv)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->finished_philos = 0;
	data->nbr_philo_must_eat = -1;
	if (argv[5])
		data->nbr_philo_must_eat = ft_atoi(argv[5]);
	if (data->nbr_philo_must_eat == 0)
		return (1);
	return (0);
}