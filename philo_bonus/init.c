/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:56:38 by egualand          #+#    #+#             */
/*   Updated: 2024/01/03 14:42:43 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	unlink_sem(void);
static int	create_process_fork(t_data *data, int index);

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
	unlink_sem();
	data->print = sem_open("/print", O_CREAT, 0644, 1);
	data->eat = sem_open("/eat", O_CREAT, 0644, 1);
	data->finish = sem_open("/finish", O_CREAT, 0644, 1);
	data->dead = sem_open("/dead", O_CREAT, 0644, 0);
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philo);
	return (data);
}

int	init_philo(t_data *data)
{
	int	i;
	int	status;

	data->t_start = get_time();
	i = -1;
	while (++i < data->n_philo)
	{
		if (create_process_fork(data, i) == 1)
			return (1);
	}
	i = -1;
	while (++i < data->n_philo)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == 200)
			kill_all(data);
	}
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

static int	create_process_fork(t_data *data, int index)
{
	data->philo[index].id = index + 1;
	data->philo[index].data = data;
	data->philo[index].t_last_meal = data->t_start;
	data->philo[index].n_eat = 0;
	data->philo[index].pid = fork();
	if (data->philo[index].pid == -1)
		return (1);
	else if (data->philo[index].pid == 0)
	{
		routine(&data->philo[index]);
		exit(0);
	}
	return (0);
}
