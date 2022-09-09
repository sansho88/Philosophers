/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 15:54:35 by tgriffit          #+#    #+#             */
/*   Updated: 2022/09/08 16:54:36 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Init each value of the t_world struct
 * @param world
 * @param argc
 * @param argv
 */
bool	init_world(t_world *world, int argc, char **argv)
{
	pthread_mutex_init(&world->check_go, NULL);
	pthread_mutex_init(&world->god_voice, NULL);
	world->go = false;
	gettimeofday(&world->start, NULL);
	world->argv = argv;
	world->argc = argc;
	if ((!check_arg_value(argv[1]) || !check_arg_value(argv[2])
			|| !check_arg_value(argv[3]) || !check_arg_value(argv[4]))
		|| (argc == 6 && !check_arg_value(argv[5])))
		return (false);
	world->nb_philos = ft_atoi(world->argv[1]);
	world->lifetime = ft_atoi(world->argv[2]);
	world->time4eat = ft_atoi(world->argv[3]);
	world->sleeptime = ft_atoi(world->argv[4]);
	if (argc == 6 && check_arg_value(argv[5]))
		world->nb_meals_max = ft_atoi(world->argv[5]);
	return (true);
}

/**
 * Init each value of the t_philo struct
 * @param philo
 * @param world
 * @return
 */
bool	give_birth_to_philo(t_philo *philo, t_world *world)
{
	philo->lifetime = world->lifetime;
	philo->time4eat = world->time4eat;
	philo->sleeptime = world->sleeptime;
	philo->nb_meals = 0;
	gettimeofday(&philo->lastmeal, NULL);
	philo->world = world;
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->has_eaten, NULL);
	if (philo->lifetime <= 0 || philo->time4eat <= 0 || philo->sleeptime <= 0)
		return (false);
	return (true);
}

/**
 * Set the tab of t_philos
 * @param cavern
 * @param world
 * @return
 */
bool	place_philos_in_cavern(t_philo *cavern, t_world *world)
{
	size_t	i;

	i = 0;
	while (i < world->nb_philos)
	{
		if (!give_birth_to_philo(&cavern[i], world))
			return (false);
		cavern[i].id = i + 1;
		i++;
	}
	i = 0;
	while (i < world->nb_philos)
	{
		if (world->nb_philos > 1)
			cavern[i].nextfork = &cavern[(i + 1) % world->nb_philos].fork;
		i++;
	}
	return (true);
}

/**
 * Check the time since the last meal of each philo and compares if they shall
 * to still be alive.\n
 * When a philo dies, the informations about its death are gathered.
 * @param void_world
 * @return
 */
void	*health_checker(void	*void_world)
{
	t_world	*world;
	long	lastmealtime;
	size_t	i;

	world = (t_world *)void_world;
	i = 0;
	while (true)
	{
		pthread_mutex_lock(&world->cavern[i].has_eaten);
		lastmealtime = ft_timer_since(world->cavern[i].lastmeal);
		if (lastmealtime >= world->cavern[i].lifetime)
		{
			pthread_mutex_unlock(&world->cavern[i].has_eaten);
			put_in_coffin(world->dead_philo, &world->cavern[i], world);
			pthread_mutex_lock(&world->check_go);
			world->go = false;
			pthread_mutex_unlock(&world->check_go);
			unchain_philos(world->cavern);
			return (NULL);
		}
		pthread_mutex_unlock(&world->cavern[i].has_eaten);
		i = ((i + 1) % world->nb_philos);
	}
}

/**
 * Launch the routine of each philos, and the health_checker until the end
 * @param cavern
 * @param world
 * @param nb_philos
 */
void	light_on_cavern(t_philo *cavern, t_world *world, size_t nb_philos)
{
	size_t		i;
	pthread_t	doctor;

	i = 0;
	pthread_mutex_lock(&world->check_go);
	while (i < nb_philos)
	{
		if (pthread_create((&cavern[i].philo), NULL, &routine, &cavern[i]) != 0)
			return ;
		i++;
	}
	gettimeofday(&world->start, NULL);
	world->cavern = cavern;
	if (world->nb_philos > 1)
		pthread_create(&doctor, NULL, &health_checker, world);
	else
	{
		doctor = 0;
		world->dead_philo = NULL;
	}
	world->go = true;
	pthread_mutex_unlock(&world->check_go);
	pthread_join(doctor, NULL);
}
