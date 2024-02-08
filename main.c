/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:16:27 by kcheong           #+#    #+#             */
/*   Updated: 2024/01/09 21:16:27 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
Philo test
----------
- Do not test with more than 200 philosophers
- Do not test with time_to_die or time_to_eat or time_to_sleep under 60 ms
- Test with 1 800 200 200, the philosopher should not eat and should die!
- Test with 5 800 200 200, no one should die!
- Test with 5 800 200 200 7, no one should die and the simulation should 
  stop when all the philosopher has eaten at least 7 times each.
- Test with 4 410 200 200, no one should die!
- Test with 4 310 200 100, a philosopher should die!
- Test with 2 philosophers and check the different times (a death delayed 
  by more than 10 ms is unacceptable).
- Test with your values to check all the rules. Check if a philosopher 
  dies at the right time if they don't steal forks, etc.
*/

void	clean(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_mutex_destroy(&data->forks[i].fork);
		pthread_mutex_destroy(&data->philos[i].write_lock);
		i++;
	}
	pthread_mutex_destroy(&data->check_lock);
	free(data->forks);
	free(data->philos);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		parse_input(&data, argv);
		data_init(&data);
		start_routine(&data);
		clean(&data);
	}
	else
		error_exit("Wrong input!!");
}

