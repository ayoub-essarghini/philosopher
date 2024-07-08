/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 23:28:14 by aes-sarg          #+#    #+#             */
/*   Updated: 2024/07/08 23:28:18 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	init_philos(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_meal = get_time();
		data->philos[i].meals_eaten = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].params = data;
		i++;
	}
}

void	print_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->params->t_lock);
	if (philo->params->running == 1)
	{
		printf("%ld %d %s\n", get_time() - philo->params->start_time, philo->id,
			status);
	}
	pthread_mutex_unlock(&philo->params->t_lock);
}

void	init_mutex(t_data **data)
{
	size_t	i;

	i = 0;
	(*data)->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* (*data)->n_philos);
	if ((*data)->forks == NULL)
	{
		write(2, "Error :Memory allocation failed\n", 32);
		exit(1);
	}
	while (i < (*data)->n_philos)
	{
		pthread_mutex_init(&(*data)->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&(*data)->print_lock, NULL);
	pthread_mutex_init(&(*data)->dead_lock, NULL);
	pthread_mutex_init(&(*data)->t_lock, NULL);
	(*data)->philos = (t_philo *)malloc(sizeof(t_philo) * (*data)->n_philos);
	if ((*data)->philos == NULL)
	{
		write(2, "Error :Memory allocation failed\n", 32);
		exit(1);
	}
}

void	validate_data(int ac, char **av, t_data **data)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (only_digit(av[i]) == 1)
		{
			write(2, "Error: Nan\n", 11);
			exit(1);
		}
		i++;
	}
	(*data)->n_philos = ft_atol(av[1]);
	(*data)->t_die = ft_atol(av[2]);
	(*data)->t_eat = ft_atol(av[3]);
	(*data)->t_sleep = ft_atol(av[4]);
	if ((*data)->n_philos <= 0 || (*data)->n_philos > INT_MAX)
	{
		write(2, "Error : args not valid\n", 23);
		exit(1);
	}
	check_negative(*data);
}

void	init_data(t_data **data, int ac, char **av)
{
	check_valid_args(ac);
	*data = (t_data *)malloc(sizeof(t_data));
	if (*data == NULL)
	{
		write(2, "Error :Memory allocation failed\n", 32);
		exit(1);
	}
	validate_data(ac, av, data);
	if (ac == 6)
	{
		(*data)->n_meals = ft_atoi(av[5]);
		if ((*data)->n_meals <= 0)
		{
			write(2, "Error : args not valid\n", 23);
			exit(0);
		}
	}
	else
		(*data)->n_meals = -1;
	(*data)->running = 1;
	(*data)->n_finished_meals = 0;
	init_mutex(data);
}
