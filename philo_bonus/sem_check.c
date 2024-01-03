/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:30:02 by egualand          #+#    #+#             */
/*   Updated: 2024/01/03 14:52:50 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*sem_check(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	sem_wait(philo->data->dead);
	philo->data->p_is_dead = 1;
	return (NULL);
}
