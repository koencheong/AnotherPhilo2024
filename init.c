/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:06:56 by kcheong           #+#    #+#             */
/*   Updated: 2024/02/08 18:06:56 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	parse_input(t_data *data, char **argv)
{
	data->philo_nbr = ft_atol(argv[1]);
	data->time_to_die = (ft_atol(argv[2]));
	data->time_to_eat = (ft_atol(argv[3]));
	data->time_to_sleep = (ft_atol(argv[4]));
	if (data->time_to_die < 60
		|| data->time_to_eat < 60
		|| data->time_to_sleep < 60)
		error_exit("Die/Eat/Sleep time need to be larger than 60ms.");
	if (argv[5])
		data->meals_nbr = ft_atol(argv[5]);
	else
		data->meals_nbr = -1;
}

static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->data->philo_nbr;
	philo->first_fork = &forks[(position + 1) % philo_nbr];
	philo->second_fork = &forks[position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[position];
		philo->second_fork = &forks[(position + 1) % philo_nbr];
	}
}

static void	philo_init(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < data->philo_nbr)
	{
		philo = data->philos + i;
		philo->id = i + 1;
		philo->is_dead = false;
		philo->is_full = false;
		philo->meals_counter = 0;
		philo->data = data;
		philo->start_time = get_time();
		assign_forks(philo, data->forks, i);
		pthread_mutex_init(&philo->write_lock, NULL);
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
	pthread_mutex_init(&data->full_lock, NULL);
	while (i < data->philo_nbr)
	{
		pthread_mutex_init(&data->forks[i].fork, NULL);
		data->forks[i].fork_id = i;
		i++;
	}
	philo_init(data);
}
