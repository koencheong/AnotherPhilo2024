/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:57:52 by kcheong           #+#    #+#             */
/*   Updated: 2024/02/08 17:57:52 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			wait_for_action(philo, sleep);
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
