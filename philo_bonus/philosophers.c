/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:26:58 by egualand          #+#    #+#             */
/*   Updated: 2023/12/30 18:52:04 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	take_forks(t_philo *philo);
static void	eat_meal(t_philo *philo);

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!check_args(argc, argv))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	data = init_data(argc, argv);
	if (!data)
	{
		printf("Error: init data\n");
		return (2);
	}
	if (init_philo(data))
	{
		printf("Error: init philo\n");
		return (3);
	}
	free_all(data);
	return (0);
}

void	*routine(void *param)
{
	t_philo		*philo;
	pthread_t	check_death_t;

	philo = (t_philo *)param;
	if (philo->id % 2 == 0)
		usleep(philo->data->t_eat * 1000);
	while (!is_dead(philo))
	{
		pthread_create(&check_death_t, NULL, &check_death, param);
		take_forks(philo);
		eat_meal(philo);
		if (philo->n_eat == philo->data->n_eat)
		{
			sem_wait(philo->data->finish);
			if (++philo->data->p_finish_eat == philo->data->n_philo)
				exit(2);
			sem_post(philo->data->finish);
			pthread_join(check_death_t, NULL);
			return (NULL);
		}
		pthread_detach(check_death_t);
	}
	return (NULL);
}

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_state(philo, " has taken a fork");
	if (philo->data->n_philo == 1)
	{
		usleep((philo->data->t_die * 1000) * 2);
		return ;
	}
	sem_wait(philo->data->forks);
	print_state(philo, " has taken a fork");
}

static void	eat_meal(t_philo *philo)
{
	sem_wait(philo->data->eat);
	print_state(philo, " is eating");
	philo->t_last_meal = get_time();
	philo->n_eat++;
	sem_post(philo->data->eat);
	usleep(philo->data->t_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	print_state(philo, " is sleeping");
	usleep(philo->data->t_sleep * 1000);
	print_state(philo, " is thinking");
}
