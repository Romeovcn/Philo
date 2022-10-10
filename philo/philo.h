#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;

	long			start_timestamp;
	int				is_dead;
	int				finished_philos;

	pthread_mutex_t	lock_fork;
	pthread_mutex_t	lock_dead;
	pthread_mutex_t	lock_eat;
}					t_data;

typedef struct s_list
{
	int				index;
	int				left_fork;
	int				right_fork;
	struct s_list	*next;
	struct s_list	*previous;
}					philo_list;

typedef struct philo_data
{
	int				index;
	long			last_eat_time;
	long			die_timestamp;
	int				is_dead;
	t_data			*data;
	philo_list		*fork_table;
}					p_data;
// Get table
philo_list			*ft_lstnew(int index);
philo_list			*ft_lstlast(philo_list *lst);
void				ft_lstadd_back(philo_list **lst, philo_list *new);
void				ft_lstset_previous(philo_list *lst);
void				set_circular(philo_list *lst);
// Fork
void				drop_left_fork(philo_list *lst, int index);
void				drop_right_fork(philo_list *lst, int index);
int					take_left_fork(philo_list *lst, int index, p_data *philo);
int					take_right_fork(philo_list *lst, int index, p_data *philo);
// Free exit
void				free_philo_list(philo_list *lst, int size);
// Main
int					check_dead(t_data *data);

#endif