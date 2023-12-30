/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:58:31 by egualand          #+#    #+#             */
/*   Updated: 2023/12/30 15:33:04 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_philosopher_death(t_philo *philo, int state)
{
	sem_wait(philo->data->dead);
	philo->data->p_is_dead = state;
	sem_post(philo->data->dead);
}

int	is_dead(t_philo *philo)
{
	sem_wait(philo->data->dead);
	if (philo->data->p_is_dead)
	{
		sem_post(philo->data->dead);
		return (1);
	}
	sem_post(philo->data->dead);
	return (0);
}

void	*check_death(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	usleep((philo->data->t_die * 1000) + 1000);
	sem_wait(philo->data->eat);
	sem_wait(philo->data->finish);
	if (!is_dead(philo) && get_time()
		- philo->t_last_meal >= philo->data->t_die)
	{
		print_state(philo, " is dead");
		set_philosopher_death(philo, 1);
	}
	sem_post(philo->data->eat);
	sem_post(philo->data->finish);
	return (NULL);
}
