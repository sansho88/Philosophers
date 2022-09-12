/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secretary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 15:32:21 by tgriffit          #+#    #+#             */
/*   Updated: 2022/09/12 11:10:37 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Read safely a value which can be read by multiple threads at the same time,
 * by using its specific mutex
 * @param mutex
 * @param value_to_check
 * @return
 */
bool	try_to_use(pthread_mutex_t *mutex, const bool *value_to_check)
{
	bool	answer;

	pthread_mutex_lock(mutex);
	answer = *value_to_check;
	pthread_mutex_unlock(mutex);
	return (answer);
}

/**
 * Print a message, with a timestamp, without a risk of scrambled messages
 * @param msg
 * @param philo
 * @param world
 */
void	print_act(char *msg, t_philo philo, t_world *world)
{
	if (!try_to_use(&world->check_go, &world->go))
		return ;
	pthread_mutex_lock(&world->god_voice);
	printf("%ld\t%i %s\n", get_timestamp(*world), philo.id, msg);
	pthread_mutex_unlock(&world->god_voice);
}

/**
 * Check if all arguments are okay with the subject
 * @param argc
 * @param argv
 * @return
 */
bool	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Correct usage: ./philo [number_of_philosophers] [time_to_die] "
			"[time_to_eat] [time_to_sleep] "
			"(number_of_times_each_philosopher_must_eat)\n");
		return (false);
	}
	if (!ft_str_isdigit(argv + 1, argc - 1))
	{
		printf("At least one of the args isn't a number. "
			"Be kind, philos aren't aliens.\n");
		return (false);
	}
	return (true);
}

bool	check_arg_value(const char *argv)
{
	if (ft_atoi(argv) == 0 || ft_strchr(argv, '-'))
		return (false);
	if (ft_strchr(argv, '-'))
		return (false);
	if (ft_atoi(argv) < 0 || ft_atoi(argv) > 2147483647)
		return (false);
	return (true);
}
