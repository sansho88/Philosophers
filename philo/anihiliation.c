/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 14:48:39 by tgriffit          #+#    #+#             */
/*   Updated: 2022/09/12 11:38:20 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Free the pilos and destroy their chains. Mutexes are destroyed here.\n
 * If a philo is dead (#health_checker), his death message is sent.
 * @param cavern
 */
void	show_real_world(t_philo *cavern)
{
	size_t	i;

	i = 0;
	if (cavern->world->nb_philos == 1)
		pthread_join(cavern[0].philo, NULL);
	while (i < cavern->world->nb_philos)
	{
		pthread_mutex_destroy(&cavern[i].fork);
		pthread_mutex_destroy(&cavern[i].has_eaten);
		i++;
	}
	if (cavern->world->dead_philo != NULL)
		announce_death_to_family(*cavern->world->dead_philo, cavern->world);
	pthread_mutex_destroy(&cavern->world->check_go);
	pthread_mutex_destroy(&cavern->world->god_voice);
}

/**
 * Save the death's informations of the dead philo.
 * @param corpse
 * @param philo
 * @param world
 */
void	put_in_coffin(t_corpse *corpse, t_philo *philo, t_world	*world)
{
	if (philo->nb_meals != world->nb_meals_max || world->nb_meals_max == 0)
	{
		corpse->dead_philo = philo;
		corpse->time_of_death = get_timestamp(*world);
		world->dead_philo = corpse;
	}
}

/**
 * Announce the death of the philo.
 * @param corpse
 * @param world
 */
void	announce_death_to_family(t_corpse corpse, t_world *world)
{
	if (corpse.dead_philo != NULL)
	{
		pthread_mutex_lock(&world->god_voice);
		printf("[%ld]\t%i %s\n", corpse.time_of_death, corpse.dead_philo->id,
			"died");
		pthread_mutex_unlock(&world->god_voice);
	}
}

void	unchain_philos(t_philo	*cavern)
{
	size_t	i;
	size_t	max_philos;

	max_philos = cavern->world->nb_philos;
	i = 0;
	while (i < max_philos)
	{
		pthread_detach(cavern[i].philo);
		i++;
	}
}
