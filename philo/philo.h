/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvincent  <rvincent@student.42.fr   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by rvincent          #+#    #+#             */
/*   Updated: 2022/10/10 18:28:43 by rvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int				philos_nbr;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_philo_must_eat;

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
}					t_philo_list;

typedef struct philo_data
{
	int				index;
	long			last_eat_time;
	long			die_timestamp;
	int				nbr_of_eat;
	t_data			*data;
	t_philo_list	*fork_table;
}					t_philo_data;

// Lst utils
t_philo_list		*ft_lstnew(int index);
t_philo_list		*ft_lstlast(t_philo_list *lst);
int					ft_lstadd_back(t_philo_list **lst, t_philo_list *new);
void				ft_lstset_previous(t_philo_list *lst);
void				set_circular(t_philo_list *lst);
// Parsing
long				ft_atoi(const char *str);
int					is_not_numeric(char *s);
int					is_more_10_char(char *s);
int					check_error(int argc, char **argv);
// Routine utils
void				go_to_sleep(long timestamp_to_wait, t_data *data);
int					check_dead(t_data *data);
int					print_action(t_philo_data *philo, char *message,
						struct timeval current_time);
// Fork
void				drop_left_fork(t_philo_list *lst, int index);
void				drop_right_fork(t_philo_list *lst, int index);
int					take_left_fork(t_philo_list *lst, int index,
						t_philo_data *philo);
int					take_right_fork(t_philo_list *lst, int index,
						t_philo_data *philo);
// Main
int					check_dead(t_data *data);
// Check philos
void				check_philo_death(t_philo_data *philo);
// Get data
int					get_data(t_data *data, char **argv);
int					get_table(t_philo_list **fork_table,
						int philos_nbr);
// Routine
void				start_routine(t_philo_data *philo);
int					init_routine(t_philo_data *philo);
void				*philo_thread_func(void *p);
int					init_threads(t_data *data, t_philo_list *fork_table,
						pthread_t *philo_thread, t_philo_data *philo_data);
void				join_threads(pthread_t *philo_thread,
						int philos_nbr);
// Free exit
void				free_philo_list(t_philo_list *lst, int size);
void				free_and_exit(t_data data, t_philo_list *fork_table,
						pthread_t *philo_thread, t_philo_data *philo_data);
void				thread_error_join(pthread_t *philo_thread, int i);

#endif