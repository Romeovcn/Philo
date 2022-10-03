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
int					take_left_fork(philo_list *lst, int index);
int					take_right_fork(philo_list *lst, int index);
void				drop_left_fork(philo_list *lst, int index);
void				drop_right_fork(philo_list *lst, int index);

#endif