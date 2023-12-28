/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:27:22 by egualand          #+#    #+#             */
/*   Updated: 2023/12/28 17:46:40 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				n_eat;
	long int	t_last_meal;
	struct s_data	*data;
	pthread_t		thread;
	pthread_mutex_t	fork_l;
	pthread_mutex_t	*fork_r;
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
	pthread_mutex_t	print;
	pthread_mutex_t	eat;
	pthread_mutex_t	finish;
	pthread_mutex_t	dead;
}					t_data;

#endif