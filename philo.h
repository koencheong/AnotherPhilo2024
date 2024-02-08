/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcheong <kcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:51:09 by kcheong           #+#    #+#             */
/*   Updated: 2024/01/08 21:51:09 by kcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> // printf
# include <stdlib.h> // malloc free
# include <unistd.h> // usleep
# include <stdbool.h>
# include <pthread.h> // mutex: init destroy lock unlock; 
						// threads: create join detach
# include <sys/time.h> // gettimeofday
# include <limits.h> // INT_MAX

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}	t_fork;

typedef struct s_data
{
	long			philo_nbr;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meals_nbr;
	int				all_full;
	t_fork			*forks;
	t_mtx			check_lock;
	t_mtx			check2_lock;
	t_mtx			debug;
	t_mtx			full_lock;
	struct s_philo	*philos;
}	t_data;

typedef struct s_philo
{
	int			id;
	long		meals_counter;
	int			is_full;
	bool		is_dead;
	long		last_meal_time;
	long		start_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_data		*data;
	t_mtx		write_lock;
}	t_philo;

// utils
void	error_exit(const char *error);
long	ft_atol(const char *str);
long	get_time(void);

// init
void	parse_input(t_data *data, char **argv);
void	data_init(t_data *data);

// philo
void	start_routine(t_data *data);
void	*routine(void *philo_passed);
void	*lonely_philo(void *philo_passed);

// check dead or full
void	check_is_dead(t_data *data);
int		show_died(t_data *data, long last_meal, long time_to_die, int i);
int		check_is_full(t_data *data, int nbr_of_philo, int cont, int i);

// actions
int		anyone_died_or_full(t_philo *philo);
void	write_message(t_philo *philo, char *message);
void	philo_eat(t_philo *philo, long eat);
void	wait_for_action(t_philo *philo, long time);
void	*safe_malloc(size_t size);

#endif