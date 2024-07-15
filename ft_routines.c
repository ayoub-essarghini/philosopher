/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 23:27:22 by aes-sarg          #+#    #+#             */
/*   Updated: 2024/07/15 00:16:22 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	routine_2(t_philo *philo, t_data *data)
{
	take_fork_and_eat(philo, data);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	if (data->n_meals != -1 && philo->meals_eaten >= data->n_meals)
	{
		pthread_mutex_lock(&data->t_lock);
		data->n_finished_meals++;
		if (data->n_finished_meals >= data->n_philos)
			data->running = 0;
		pthread_mutex_unlock(&data->t_lock);
		return (-1);
	}
	print_status(philo, "is sleeping");
	my_usleep(data->t_sleep);
	p_sleep(philo);
	return (0);
}

int	is_died(t_data *data)
{
	pthread_mutex_lock(&data->t_lock);
	if (data->running != 1)
	{
		pthread_mutex_unlock(&data->t_lock);
		return (0);
	}
	pthread_mutex_unlock(&data->t_lock);
	return (1);
}

void	*ft_routine(void *params)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)params;
	data = philo->params;
	while (1)
	{
		if (is_died(data) == 0)
			break ;
		take_first_fork(philo);
		if (data->n_philos == 1)
		{
			my_usleep(data->t_die);
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		if (routine_2(philo, data) == -1)
			break ;
	}
	return (NULL);
}

int	check_philos(t_data *data)
{
	size_t	i;
	long	t_lst_meal;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_lock(&data->t_lock);
		t_lst_meal = get_time() - data->philos[i].last_meal;
		if (t_lst_meal > data->t_die)
		{
			printf("%ld %d %s\n", get_time()
				- data->philos[i].params->start_time, data->philos[i].id,
				"died");
			data->running = 0;
			pthread_mutex_unlock(&data->t_lock);
			return (1);
		}
		pthread_mutex_unlock(&data->t_lock);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	while (1)
	{
		if (check_philos(data))
			return (NULL);
		pthread_mutex_lock(&data->t_lock);
		if (!data->running)
		{
			pthread_mutex_unlock(&data->t_lock);
			break ;
		}
		pthread_mutex_unlock(&data->t_lock);
	}
	return (NULL);
}
