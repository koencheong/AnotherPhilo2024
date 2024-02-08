/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dead_full.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:04:46 by kcheong           #+#    #+#             */
/*   Updated: 2024/02/08 18:21:36 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_is_dead(t_data *data)
{
	int		i;
	int		cont;
	long	time_to_die;
	int		nbr_of_philo;
	long	last_meal;

	cont = 1;
	while (cont == 1)
	{
		i = 0;
		pthread_mutex_lock(&data->debug);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->debug);
		pthread_mutex_lock(&data->check_lock);
		time_to_die = data->time_to_die;
		nbr_of_philo = data->philo_nbr;
		pthread_mutex_unlock(&data->check_lock);
		while (cont && (i < nbr_of_philo))
		{
			if (show_died(data, last_meal, time_to_die, i) == 0)
				cont = 0;
			cont = check_is_full(data, nbr_of_philo, cont, i);
			i++;
		}
	}
}

int	show_died(t_data *data, long last_meal, long time_to_die, int i)
{
	if (get_time() - last_meal > time_to_die)
	{
		write_message(&data->philos[i], "died");
		pthread_mutex_lock(&data->check2_lock);
		data->philos[i].is_dead = true;
		pthread_mutex_unlock(&data->check2_lock);
		return (0);
	}
	return (1);
}

int	check_is_full(t_data *data, int nbr_of_philo, int cont, int i)
{
	int	is_full;

	pthread_mutex_lock(&data->full_lock);
	is_full = data->philos[i].is_full;
	pthread_mutex_unlock(&data->full_lock);
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
	return (cont);
}
