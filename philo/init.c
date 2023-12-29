/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:56:38 by egualand          #+#    #+#             */
/*   Updated: 2023/12/29 16:42:06 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	data->n_eat = 0;
	if (argc == 6)
		data->n_eat = ft_atoi(argv[5]);
	data->philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philo)
		return (NULL);
	data->p_finish_eat = 0;
	data->p_is_dead = 0;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->eat, NULL);
	pthread_mutex_init(&data->dead, NULL);
	pthread_mutex_init(&data->finish, NULL);
	return (data);
}

int	init_philo(t_data *data)
{
	int	i;

	data->t_start = get_time();
	i = -1;
	while (++i < data->n_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].data = data;
		data->philo[i].t_last_meal = data->t_start;
		data->philo[i].n_eat = 0;
		pthread_mutex_init(&data->philo[i].fork_l, NULL);
		data->philo[i].fork_r = &data->philo[(i + 1) % data->n_philo].fork_l;
		if (pthread_create(&data->philo[i].thread, NULL, &routine,
				&data->philo[i]))
			return (1);
	}
	i = -1;
	while (++i < data->n_philo)
		if (pthread_join(data->philo[i].thread, NULL))
			return (1);
	return (0);
}

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->philo[i].fork_l);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->eat);
	pthread_mutex_destroy(&data->dead);
	pthread_mutex_destroy(&data->finish);
	free(data->philo);
	free(data);
}
