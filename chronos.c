/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronos.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 14:52:39 by tgriffit          #+#    #+#             */
/*   Updated: 2022/09/07 15:32:55 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Get the timestamp from the beginning of the simulation
 * @param world
 * @return
 */
long	get_timestamp(t_world world)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000)
		- (world.start.tv_sec * 1000 + world.start.tv_usec / 1000));
}

/**
 * Get the differnece of time, in ms, between the start timeval and the
 * actual time
 * @param start
 * @return
 */
long	ft_timer_since(struct timeval start)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000)
		- (start.tv_sec * 1000 + start.tv_usec / 1000));
}

/**
 * A revisited usleep, more precise and also enough optimized for this project
 * @param time
 */
void	myusleep(useconds_t time)
{
	struct timeval	temps;
	struct timeval	start;
	useconds_t		timestamp;

	gettimeofday(&temps, NULL);
	start = temps;
	timestamp = (temps.tv_sec * 1000 + temps.tv_usec / 1000)
		- (start.tv_sec * 1000 + start.tv_usec / 1000);
	while (timestamp < time)
	{
		usleep(100);
		gettimeofday(&temps, NULL);
		timestamp = (temps.tv_sec * 1000 + temps.tv_usec / 1000)
			- (start.tv_sec * 1000 + start.tv_usec / 1000);
	}
}
