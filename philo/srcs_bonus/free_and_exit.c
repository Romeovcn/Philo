#include "philo_bonus.h"

void free_and_exit(t_data data)
{
	free(data.pid);
	sem_close(data.sem_death);
	sem_close(data.sem_fork);
	sem_close(data.sem_exit);
	sem_close(data.sem_last_eat_time);
	sem_close(data.sem_exit_mutex);
	sem_unlink("sem_death");
	sem_unlink("sem_fork");
	sem_unlink("sem_exit");
	sem_unlink("sem_last_eat_time");
	sem_unlink("sem_exit_mutex");
}