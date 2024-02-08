#include "philo.h"

void	write_message(t_philo *philo, char *message)
{
	if (anyone_died_or_full(philo) == true)
		return ;
	else
	{
		pthread_mutex_lock(&philo->write_lock);
		printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, message);
		pthread_mutex_unlock(&philo->write_lock);
	}
}