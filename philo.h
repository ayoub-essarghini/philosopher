/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 23:27:59 by aes-sarg          #+#    #+#             */
/*   Updated: 2024/07/08 23:28:02 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct philo
{
	int				id;
	long			last_meal;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct data		*params;
}					t_philo;

typedef struct data
{
	size_t			n_philos;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	int				n_meals;
	size_t			n_finished_meals;
	long			start_time;
	pthread_mutex_t	t_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	dead_lock;
	int				running;
	t_philo			*philos;
}					t_data;

int				init_data(t_data **data, int ac, char **av);
void			*monitor_routine(void *args);
void			*ft_routine(void *params);
long			get_time(void);
int				my_usleep(size_t millis);
int				check_valid_args(int ac);
void			init_philos(t_data *data);
long			ft_atol(const char *nptr);
int				ft_atoi(const char *nptr);
void			print_status(t_philo *philo, const char *status);
int				only_digit(char *str);
int				check_negative(t_data *data);
void			take_fork_and_eat(t_philo *philo, t_data *data);
void			take_first_fork(t_philo *philo);

#endif
