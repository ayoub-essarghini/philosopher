/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 23:27:49 by aes-sarg          #+#    #+#             */
/*   Updated: 2024/07/08 23:27:52 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	ft_clean_up(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->t_lock);
	free(data->forks);
	free(data->philos);
	free(data);
}

int	main(int ac, char **av)
{
	t_data		*data;
	size_t		i;
	pthread_t	controller_th;

	i = 0;
	init_data(&data, ac, av);
	init_philos(data);
	data->start_time = get_time();
	pthread_create(&controller_th, NULL, monitor_routine, data);
	while (i < data->n_philos)
	{
		pthread_create(&data->philos[i].thread, NULL, ft_routine,
			&data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(controller_th, NULL);
	ft_clean_up(data);
}
