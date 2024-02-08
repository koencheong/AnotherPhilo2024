#include "philo.h"

void	write_message(t_philo *philo, char *message)
{
	if (anyoneDied(philo) == true)
		return ;
	else
	{
		pthread_mutex_lock(&philo->write_lock);
		printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, message);
		pthread_mutex_unlock(&philo->write_lock);
	}
}

void	checkIsDead(t_data *data)
{
	int	i;
	int	cont;
	long time_to_die;
	int	nbr_of_philo;
	long	last_meal;

	cont = 1;
	while (cont == 1)
	{
		i = 0;
		pthread_mutex_lock(&data->check_lock);
		time_to_die = data->time_to_die;
		nbr_of_philo = data->philo_nbr;
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->check_lock);
		while (cont && (i < nbr_of_philo))
		{
			if (get_time() - last_meal > time_to_die)
			{
				write_message(&data->philos[i], "died");
				pthread_mutex_lock(&data->check2_lock);
				data->philos[i].isDead = true;
				pthread_mutex_unlock(&data->check2_lock);
				cont = 0;
			}
			i++;
		}
	}
}

int	anyoneDied(t_philo *philo)
{
	int	ret;
	int	i;
	int	n;

	ret = 0;
	i = 0;
	pthread_mutex_lock(&philo->data->check_lock);
	n = philo->data->philo_nbr;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (i < n)
	{
		pthread_mutex_lock(&philo->data->check2_lock);
		if (philo->data->philos[i].isDead == true)
			ret = 1;
		pthread_mutex_unlock(&philo->data->check2_lock);
		i++;
	}
	return (ret);
}

void	waitForAction(t_philo *philo, long time)
{
	long	start;

	start = get_time();
	while (anyoneDied(philo) != true)
	{
		if (get_time() - start >= time)
			return ;
		ft_usleep(20);
	}
	return ;
}

void	*routine(void *philo_passed)
{
	t_philo *philo;
	long	eat;
	long	sleep;
	
	philo = (t_philo *) philo_passed;
	pthread_mutex_lock(&philo->data->check_lock);
	eat = philo->data->time_to_eat;
	sleep = philo->data->time_to_sleep;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (anyoneDied(philo) != true)
	{
		if (anyoneDied(philo) != true)
		{
			pthread_mutex_lock(&philo->first_fork->fork);
			write_message(philo, "has taken a fork");
			pthread_mutex_lock(&philo->second_fork->fork);
			write_message(philo, "has taken a fork");
			write_message(philo, "is eating");
			pthread_mutex_lock(&philo->data->check_lock);
			philo->last_meal_time = get_time();
			// philo->meals_counter++;
			// CHECK MEAL NUMBER
			pthread_mutex_unlock(&philo->data->check_lock);
			waitForAction(philo, eat);
			pthread_mutex_unlock(&philo->first_fork->fork);
			pthread_mutex_unlock(&philo->second_fork->fork);
		}

		if (anyoneDied(philo) != true)
		{
			write_message(philo, "is sleeping");
			waitForAction(philo, sleep);
			write_message(philo, "is thinking");
			// printf("done here?\n");
		}
		// printf("Philo %d done!\n", philo->id);
	}
	// printf("Philo %d exited!\n", philo->id);
	return (NULL);
}

void	start_routine(t_data *data)
{
	int	i;

	i = 0;
	if (data->meals_nbr == 0)
		return ;
	else if (data->meals_nbr == 1)
		; //TODO
	else
	{
		while (i < data->philo_nbr)
		{
			// printf("i is %d\n", i);
			pthread_create(&data->philos[i].thread_id, NULL, &routine, &data->philos[i]);
			i++;
			usleep(100);
		}
	}
	// printf("heree\n");
	checkIsDead(data);
	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		// printf("Joining philo %d......\n", data->philos->id);
		i++;
	}
}

/*
	int	meals_eaten;
	int	required_meals;

		meals_eaten = data->philos[i].meals_counter;
		required_meals = data->meals_nbr;

			if (meals_eaten == required_meals)
			{
				pthread_mutex_lock(&data->check2_lock);
				data->philos[i].isFull = true;
				pthread_mutex_unlock(&data->check2_lock);
				cont = 0;
			}
			
			*/