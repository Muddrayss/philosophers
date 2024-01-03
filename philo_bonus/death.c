/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:58:31 by egualand          #+#    #+#             */
/*   Updated: 2024/01/03 14:50:42 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	post_all(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		sem_post(data->dead);
	i = -1;
	while (++i < data->n_philo)
		sem_post(data->eat);
	i = -1;
	while (++i < data->n_philo)
		sem_post(data->finish);
	i = -1;
	while (++i < data->n_philo)
		sem_post(data->forks);
}

static void	free_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_close(philo->data->print);
	sem_close(philo->data->eat);
	sem_close(philo->data->finish);
	sem_close(philo->data->dead);
	sem_close(philo->data->forks);
	free(philo->data->philo);
	free(data);
}

void	set_philosopher_death(t_philo *philo, int state)
{
	if (state == 190)
	{
		print_state(philo, "is dead");
		post_all(philo->data);
		return ;
	}
	free_philo(philo);
	exit(state);
}

void	*check_death(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	usleep((philo->data->t_die * 1000) + 7000);
	sem_wait(philo->data->eat);
	sem_wait(philo->data->finish);
	if (get_time() - philo->t_last_meal >= philo->data->t_die
		&& !philo->data->p_is_dead)
		set_philosopher_death(philo, 190);
	sem_post(philo->data->eat);
	sem_post(philo->data->finish);
	return (NULL);
}
