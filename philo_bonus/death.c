/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:58:31 by egualand          #+#    #+#             */
/*   Updated: 2024/01/02 16:18:37 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	free_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_close(philo->data->print);
	sem_close(philo->data->eat);
	sem_close(philo->data->finish);
	sem_close(philo->data->forks);
	free(philo->data->philo);
	free(data);
}

void	set_philosopher_death(t_philo *philo, int state)
{
	free_philo(philo);
	exit(state);
}

void	*check_death(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	usleep((philo->data->t_die * 1000) + 5000);
	sem_wait(philo->data->eat);
	sem_wait(philo->data->finish);
	if (get_time() - philo->t_last_meal >= philo->data->t_die)
	{
		print_state(philo, "is dead");
		set_philosopher_death(philo, 200);
	}
	sem_post(philo->data->eat);
	sem_post(philo->data->finish);
	return (NULL);
}
