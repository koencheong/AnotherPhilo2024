/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:18:05 by kcheong           #+#    #+#             */
/*   Updated: 2024/01/09 21:18:05 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

long	ft_atol(const char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	while (((str[i] >= 9) && (str[i] <= 13)) || (str[i] == 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			error_exit("Only positive value is allowed.");
	if (!is_digit(str[i]))
		error_exit("The input is not a numeric value.");
	while (str[i] >= '0' && str[i] <= '9')
		num = (num * 10) + str[i++] - '0';
	if (str[i] != '\0')
		error_exit("The input is not a numeric value.");
	if (ft_strlen(str) > 10 || num > INT_MAX)
		error_exit("The input is too big.");
	return (num);
}

void	parse_input(t_table *table, char **argv)
{
	table->philo_nbr = ft_atol(argv[1]);
	table->time_to_die = (ft_atol(argv[2])) * 1000;
	table->time_to_eat = (ft_atol(argv[3]) * 1000);
	table->time_to_sleep = (ft_atol(argv[4]) * 1000);
	if (table->time_to_die < 60000
		|| table->time_to_eat < 60000
		|| table->time_to_sleep < 60000)
		error_exit("Die/Eat/Sleep time need to be larger than 60ms.");
	if (argv[5])
		table->meals_nbr = ft_atol(argv[5]);
	else
		table->meals_nbr = -1;
}
