/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:57:45 by kcheong           #+#    #+#             */
/*   Updated: 2024/02/08 18:09:00 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_message(t_philo *philo, char *message)
{
	if (anyone_died_or_full(philo) == true)
		return ;
	else
	{
		pthread_mutex_lock(&philo->write_lock);
		printf("%ld %d %s\n", get_time() - philo->start_time,
			philo->id, message);
		pthread_mutex_unlock(&philo->write_lock);
	}
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
	wait_for_action(philo, eat);
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

void	wait_for_action(t_philo *philo, long time)
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

int	anyone_died_or_full(t_philo *philo)
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
		if (philo->data->philos[i].is_dead == true)
			ret = 1;
		pthread_mutex_unlock(&philo->data->check2_lock);
		i++;
	}
	return (ret);
}

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
		error_exit("Malloc error!");
	return (ret);
}
