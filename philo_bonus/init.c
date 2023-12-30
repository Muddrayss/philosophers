/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:56:38 by egualand          #+#    #+#             */
/*   Updated: 2023/12/30 18:59:13 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	unlink_sem(void);

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
	data->print_override = 1;
	unlink_sem();
	data->print = sem_open("/print", O_CREAT, 0644, 1);
	data->eat = sem_open("/eat", O_CREAT, 0644, 1);
	data->finish = sem_open("/finish", O_CREAT, 0644, 1);
	data->dead = sem_open("/dead", O_CREAT, 0644, 1);
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philo);
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
		data->philo[i].pid = fork();
		if (data->philo[i].pid == -1)
			return (1);
		else if (data->philo[i].pid == 0)
		{
			routine(&data->philo[i]);
			exit(0);
		}
		usleep(100);
	}
	i = -1;
	waitpid(-1, NULL, 0);
	printf("All philosophers have eaten %d times\n", data->n_eat);
	return (0);
}

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		kill(data->philo[i].pid, SIGKILL);
	sem_close(data->print);
	sem_close(data->eat);
	sem_close(data->finish);
	sem_close(data->dead);
	sem_close(data->forks);
	free(data->philo);
	free(data);
}

static void	unlink_sem(void)
{
	sem_unlink("/print");
	sem_unlink("/eat");
	sem_unlink("/finish");
	sem_unlink("/dead");
	sem_unlink("/forks");
}
