#include "philo.h"
#include <stdio.h>

static void assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->data->philo_nbr;
	philo->first_fork = &forks[(position + 1) % philo_nbr];
	philo->second_fork = &forks[position];
	if (philo->id % 2 == 0)
	{
		// printf("Philo position is %d\n", position);
		philo->first_fork = &forks[position];
		philo->second_fork = &forks[(position + 1) % philo_nbr];
	}
}

static void philo_init(t_data *data)
{
	int i;
	t_philo *philo;

	i = 0;
	while (i < data->philo_nbr)
	{
		philo = data->philos + i;
		philo->id = i + 1;
		philo->isDead = false;
		philo->isFull = false;
		philo->meals_counter = 0;
		philo->data = data;
		philo->start_time = get_time();
		assign_forks(philo, data->forks, i);
		pthread_mutex_init(&philo->write_lock, NULL);
		// printf("Philo %d first fork is %d second fork is %d\n", philo->id, philo->first_fork->fork_id, philo->second_fork->fork_id);
		i++;
	}
}

void	data_init(t_data *data)
{
	int	i;

	i = 0;
	data->philos = safe_malloc(sizeof(t_philo) * data->philo_nbr);
	data->forks = safe_malloc(sizeof(t_fork) * data->philo_nbr);
	data->all_full = 0;
	pthread_mutex_init(&data->check_lock, NULL);
	pthread_mutex_init(&data->check2_lock, NULL);
	pthread_mutex_init(&data->debug, NULL);
	pthread_mutex_init(&data->full, NULL);
	while (i < data->philo_nbr)
	{
		pthread_mutex_init(&data->forks[i].fork, NULL);
		data->forks[i].fork_id = i;
		i++;
	}
	philo_init(data);
}
