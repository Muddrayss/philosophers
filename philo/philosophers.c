/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:26:58 by egualand          #+#    #+#             */
/*   Updated: 2023/12/28 17:55:32 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			check_args(int argc, char **argv);
int			ft_atoi(const char *nptr);
void		free_all(t_data *data);
t_data		*init_data(int argc, char **argv);
int			init_philo(t_data *data);
long int	get_time(void);
void		*routine(void *param);
void		set_philosopher_death(t_philo *philo, int state);
int			is_dead(t_philo *philo);
void		*check_death(void *param);
void		take_forks(t_philo *philo);
void		eat_meal(t_philo *philo);

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!check_args(argc, argv))
		return (1);
	data = init_data(argc, argv);
	if (!data)
		return (2);
	if (init_philo(data))
		return (3);
	free_all(data);
	return (0);
}

int	check_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (0);
	if (ft_atoi(argv[1]) < 1)
		return (0);
	if (ft_atoi(argv[2]) < 0)
		return (0);
	if (ft_atoi(argv[3]) < 0)
		return (0);
	if (argc == 6 && ft_atoi(argv[4]) < 1)
		return (0);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		is_neg;
	long	num;

	i = 0;
	is_neg = 1;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			is_neg *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num *= 10;
		num += nptr[i] - 48;
		i++;
	}
	return ((int)(num * is_neg));
}

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->philo[i].fork_l);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->eat);
	free(data->philo);
	free(data);
}

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]) * 1000;
	data->t_eat = ft_atoi(argv[3]) * 1000;
	data->t_sleep = ft_atoi(argv[4]) * 1000;
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

long int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	*routine(void *param)
{
	t_philo		*philo;
	pthread_t	check_death_t;

	philo = (t_philo *)param;
	if (philo->id % 2 != 0)
		usleep(philo->data->t_eat);
	while (!is_dead(philo))
	{
		pthread_create(&check_death_t, NULL, &check_death, param);
		pthread_detach(check_death_t);
		if (++philo->n_eat == philo->data->n_eat)
		{
			pthread_mutex_lock(&philo->data->finish);
			if (++philo->data->p_finish_eat == philo->data->n_philo)
				set_philosopher_death(philo, 2);
			pthread_mutex_unlock(&philo->data->finish);
			return (NULL);
		}
	}
	return (NULL);
}

void	set_philosopher_death(t_philo *philo, int state)
{
	pthread_mutex_lock(&philo->data->dead);
	philo->data->p_is_dead = state;
	pthread_mutex_unlock(&philo->data->dead);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead);
	if (philo->data->p_is_dead)
	{
		pthread_mutex_unlock(&philo->data->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead);
	return (0);
}

void	print_state(t_philo *philo, char *str)
{
	long long	elapsed_time;

	pthread_mutex_lock(&philo->data->print);
	elapsed_time = get_time() - philo->data->t_start;
	if (philo->data->p_is_dead && elapsed_time >= 0 && elapsed_time <= INT_MAX
		&& is_dead(philo))
		printf("%lld %d %s\n", elapsed_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
}

void	*check_death(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	usleep(philo->data->t_die + 1);
	pthread_mutex_lock(&philo->data->eat);
	pthread_mutex_lock(&philo->data->finish);
	take_forks(philo);
	eat_meal(philo);
	if (!is_dead(philo) && get_time()
		- philo->t_last_meal >= philo->data->t_die)
	{
		print_state(philo, " is dead\n");
		set_philosopher_death(philo, 1);
	}
	pthread_mutex_lock(&philo->data->eat);
	pthread_mutex_lock(&philo->data->finish);
	return (NULL);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l);
	print_state(philo, " has taken a fork\n");
	pthread_mutex_lock(philo->fork_r);
	print_state(philo, " has taken a fork\n");
}

void	eat_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->eat);
	print_state(philo, " is eating\n");
	philo->t_last_meal = get_time();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->data->eat);
	usleep(philo->data->t_eat);
	pthread_mutex_unlock(&philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
	print_state(philo, " is sleeping\n");
	usleep(philo->data->t_sleep);
	print_state(philo, " is thinking");
}