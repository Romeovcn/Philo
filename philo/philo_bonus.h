/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/14 19:12:13 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/signal.h>
typedef struct s_data
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nbr_philo_must_eat;

	int		index;
	int		finished_philos;

	pid_t	*pid;
	sem_t	*sem_fork;
	sem_t	*sem_death;
	sem_t	*sem_exit;
	sem_t	*sem_last_eat_time;
	sem_t	*sem_exit_mutex;
}			t_data;

typedef struct philo_data
{
	int		index;
	long	last_eat_time;
	long	die_timestamp;
	int		nbr_of_eat;
	int		exit;
	t_data	*data;
}			t_philo_data;

// Parsing
long		ft_atoi(const char *str);
int			is_not_numeric(char *s);
int			is_more_10_char(char *s);
int			check_error(int argc, char **argv);
// Get data
int			get_data(t_data *data, char **argv);
// Free and exit
void		free_and_exit(t_data data);

#endif