#include "philo.h"

void	check_is_dead(t_data *data)
{
	int	i;
	int	cont;
	long	time_to_die;
	int	nbr_of_philo;
	long	last_meal;
	int	is_full;

	cont = 1;
	while (cont == 1)
	{
		i = 0;
		pthread_mutex_lock(&data->full_lock);
		is_full = data->philos[i].is_full;
		pthread_mutex_unlock(&data->full_lock);
		pthread_mutex_lock(&data->debug);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->debug);
		pthread_mutex_lock(&data->check_lock);
		time_to_die = data->time_to_die;
		nbr_of_philo = data->philo_nbr;
		// is_full = data->philos[i].is_full;
		pthread_mutex_unlock(&data->check_lock);
		while (cont && (i < nbr_of_philo))
		{
			if (get_time() - last_meal > time_to_die)
			{
				write_message(&data->philos[i], "died");
				pthread_mutex_lock(&data->check2_lock);
				data->philos[i].is_dead = true;
				pthread_mutex_unlock(&data->check2_lock);
				cont = 0;
			}
			if (is_full == true)
			{
				pthread_mutex_lock(&data->full_lock);
				data->all_full++;
				if (data->all_full == nbr_of_philo)
				{
					cont = 0;
					pthread_mutex_lock(&data->check2_lock);
					data->philos[i].is_dead = true;
					pthread_mutex_unlock(&data->check2_lock);
				}
				pthread_mutex_unlock(&data->full_lock);
			}
			i++;
		}
	}
}

int	anyone_died_or_full(t_philo *philo)
{
	int	ret;
	int	i;
	int	n;
	// int	is_full;

	ret = 0;
	i = 0;
	pthread_mutex_lock(&philo->data->check_lock);
	n = philo->data->philo_nbr;
	pthread_mutex_unlock(&philo->data->check_lock);
	// pthread_mutex_lock(&philo->data->full);
	// is_full = philo->data->is_full[i];
	// pthread_mutex_unlock(&philo->data->full);
	while (i < n)
	{
		pthread_mutex_lock(&philo->data->check2_lock);
		if (philo->data->philos[i].is_dead == true)
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
	while (anyone_died_or_full(philo) != true)
	{
		if (get_time() - start >= time)
			return ;
		usleep(20);
	}
	return ;
}

void	philo_eat(t_philo *philo, long eat)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->second_fork->fork);
	write_message(philo, "has taken a fork");
	write_message(philo, "is eating");
	pthread_mutex_lock(&philo->data->debug);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->debug);
	waitForAction(philo, eat);
	pthread_mutex_lock(&philo->data->full_lock);
	philo->meals_counter++;
	if (philo->data->meals_nbr != -1)
	{
		if (philo->meals_counter > philo->data->meals_nbr)
			philo->is_full = true;
	}
	pthread_mutex_unlock(&philo->data->full_lock);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

void	*routine(void *philo_passed)
{
	t_philo	*philo;
	long	eat;
	long	sleep;

	philo = (t_philo *) philo_passed;
	pthread_mutex_lock(&philo->data->check_lock);
	eat = philo->data->time_to_eat;
	sleep = philo->data->time_to_sleep;
	pthread_mutex_unlock(&philo->data->check_lock);
	while (anyone_died_or_full(philo) != true)
	{
		if (anyone_died_or_full(philo) != true)
			philo_eat(philo, eat);

		if (anyone_died_or_full(philo) != true)
		{
			write_message(philo, "is sleeping");
			waitForAction(philo, sleep);
			write_message(philo, "is thinking");
		}
	}
	return (NULL);
}

void	*lonely_philo(void *philo_passed)
{
	t_philo	*philo;

	philo = (t_philo *) philo_passed;
	pthread_mutex_lock(&philo->data->debug);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->debug);
	pthread_mutex_lock(&philo->first_fork->fork);
	write_message(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->first_fork->fork);
	while (anyone_died_or_full(philo) != true)
		usleep(200);
	return (NULL);
}

void	start_routine(t_data *data)
{
	int	i;

	i = 0;
	if (data->philo_nbr == 1)
	{
		pthread_create(&data->philos[i].thread_id, NULL,
			&lonely_philo, &data->philos[i]);
		usleep(10);
	}
	else
	{
		while (i < data->philo_nbr)
		{
			pthread_create(&data->philos[i].thread_id,
				NULL, &routine, &data->philos[i]);
			i++;
			usleep(100);
		}
	}
	check_is_dead(data);
	i = -1;
	while (++i < data->philo_nbr)
		pthread_join(data->philos[i].thread_id, NULL);
}

/*
	int	meals_eaten;
	int	required_meals;

		meals_eaten = data->philos[i].meals_counter;
		required_meals = data->meals_nbr;

			if (meals_eaten == required_meals)
			{
				pthread_mutex_lock(&data->check2_lock);
				data->philos[i].is_full = true;
				pthread_mutex_unlock(&data->check2_lock);
				cont = 0;
			}
			
			*/