/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:27:22 by egualand          #+#    #+#             */
/*   Updated: 2023/12/30 14:57:53 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	pid_t			pid;
	int				n_eat;
	long int		t_last_meal;
	struct s_data	*data;
	pthread_t		thread;
}					t_philo;

typedef struct s_data
{
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	int				p_finish_eat;
	long int		t_start;
	int				p_is_dead;
	t_philo			*philo;
	sem_t			*print;
	sem_t			*eat;
	sem_t			*finish;
	sem_t			*dead;
	sem_t			*forks;
}					t_data;

int					ft_atoi(const char *nptr);
int					check_args(int argc, char **argv);
t_data				*init_data(int argc, char **argv);
int					init_philo(t_data *data);
void				free_all(t_data *data);
void				*routine(void *param);
void				set_philosopher_death(t_philo *philo, int state);
int					is_dead(t_philo *philo);
void				*check_death(void *param);
long int			get_time(void);
void				print_state(t_philo *philo, char *str);

#endif