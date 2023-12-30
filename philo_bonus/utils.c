/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:57:58 by egualand          #+#    #+#             */
/*   Updated: 2023/12/30 17:40:19 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Get time in milliseconds
long int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	print_state(t_philo *philo, char *str)
{
	long long	elapsed_time;

	sem_wait(philo->data->print);
	elapsed_time = get_time() - philo->data->t_start;
	if (elapsed_time >= 0 && elapsed_time <= INT_MAX && !is_dead(philo)
		&& (philo->n_eat != philo->data->n_eat || philo->data->n_eat == 0))
		printf("%lld %d %s\n", elapsed_time, philo->id, str);
	sem_post(philo->data->print);
}
