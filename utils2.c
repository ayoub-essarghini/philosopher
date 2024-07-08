/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-sarg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 23:28:27 by aes-sarg          #+#    #+#             */
/*   Updated: 2024/07/08 23:28:30 by aes-sarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '+')
			return (1);
		i++;
	}
	return (0);
}

void	check_negative(t_data *data)
{
	if (data->t_die <= 0 || data->t_eat <= 0 || data->t_sleep <= 0)
	{
		write(2, "Error : args not valid\n", 23);
		exit(1);
	}
}
