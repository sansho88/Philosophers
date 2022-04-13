/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 12:03:55 by tgriffit          #+#    #+#             */
/*   Updated: 2022/04/13 12:05:15 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *string)
{
	size_t i;

	i = 0;
	while(string[i])
		i++;
	return (i);
}

void	myusleep(useconds_t time)
{
	struct timeval	temps;
	useconds_t		start;

	gettimeofday(&temps, NULL);
	start = temps.tv_usec;
	while (temps.tv_usec - start < time)
		gettimeofday(&temps, NULL);
}

void	print_act(t_philo philosoph, const char *act)
{
	useconds_t		timestamp;
	struct timeval	realtime;
	pthread_mutex_t	mtx;

	pthread_mutex_lock(&mtx);
	gettimeofday(&realtime, NULL);
	timestamp = realtime.tv_usec / 1000 - philosoph.birth.tv_usec / 1000;
	printf("[%dms]\t%d is %sing\n",timestamp, (int)philosoph.id, act);
	pthread_mutex_unlock(&mtx);
	//printf("[%d]\t%zu", timestamp, philosoph.id);
	/*ft_putnbr_fd((int)timestamp, 1);
	write(1, " ", 1);
	ft_putnbr_fd((int)philosoph.id, 1);
	write(1, " is ", 4);
	write(1, act, ft_strlen(act));
	write(1, "ing\n", 4);*/
}

bool	eat(t_philo *philo, useconds_t time)
{
	struct timeval	timeofday;
//TODO: replace fake forks by true mutex
	gettimeofday(&timeofday, NULL);
	while (!philo->fork.isfree && !philo->nextfork->isfree)
	{
		printf("Philo[%zu] is trying to eat, but fork[%zu] is taken\n",
			philo->id, philo->id + 1);
		myusleep(1);
		if (timeofday.tv_usec - philo->lastmeal.tv_usec >= philo->starve)
		{
			philo->isalive = false;
			printf("[%d]\t%d died\n", (timeofday.tv_usec / 1000 - philo->birth.tv_usec / 1000),
				(int)philo->id);
			return (false);
		}
	}
	philo->fork.isfree = false;
	philo->nextfork->isfree = false;
	print_act(*philo, __func__);
	gettimeofday(&philo->lastmeal, NULL);
	myusleep(time);
	philo->fork.isfree = true;
	philo->nextfork->isfree = true;
	return (true);
}

void	dream(t_philo philo, useconds_t time)
{
	struct timeval	realtime;

	gettimeofday(&realtime, NULL);
	print_act(philo, "sleep");
	myusleep(time);
}

void	think(t_philo philosoph, useconds_t time)
{
	pthread_mutex_t	mtx;
	struct timeval	realtime;

	gettimeofday(&realtime, NULL);
	pthread_mutex_lock(&mtx);
	print_act(philosoph, __func__);
	pthread_mutex_unlock(&mtx);
	myusleep(time);
}

void	*routine(void *philosoph)
{
	pthread_mutex_t	secure;
	struct timeval	time;
	t_philo			philo;

	philo = (*(t_philo *)philosoph);
	pthread_mutex_init(&secure, NULL);
	gettimeofday(&time, NULL);
	if (philo.lastmeal.tv_usec - time.tv_usec < philo.starve && philo.isalive)
	{
		if (eat(&(*(t_philo *)philosoph), 2))
		{
			dream((*(t_philo *) philosoph), 2);
			think((*(t_philo *)philosoph), 8);
		}
	}
	else
		printf("[%dms]\t%d died\n", (time.tv_usec - philo.birth.tv_usec),
			(int)philo.id);
	return (NULL);
}

bool	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		puts("Correct usage: ./philo [number_of_philosophers] [time_to_die] "
			"[time_to_eat] [time_to_sleep] "
			"(number_of_times_each_philosopher_must_eat)\n");
		return (false);
	}
	if (!ft_str_isdigit(argv + 1, argc - 1))
	{
		puts("At least one of the args isn't a number. "
			"Be kind, philos aren't aliens.\n");
		return (false);
	}
	return (true);
}

/**
 * Returns the amount of philosophers welcomed into the cavern.\n
 * If something went wrong, nobody will be allowed.
 * @param philo = Philosoph who will be checked
 * @param id = his/her ID card
 * @param argv
 * @return
 */
size_t	welcome_philos(t_philo *philo, size_t id, char **argv, int argc)
{
	if (!check_args(argc, argv))
		return (0);
	philo->id = id;
	philo->lifetime = ft_atoi(argv[2]);
	philo->starve = ft_atoi(argv[3]);
	philo->resttime = ft_atoi(argv[4]);
	if (argc == 6)
		philo->nb_meals = ft_atoi(argv[5]);
	if (philo->lifetime <= 0 || philo->starve <= 0 || philo->resttime <= 0)
		return (0);
	philo->fork.isfree = true;
	philo->isalive = true;
	gettimeofday(&philo->lastmeal, NULL);
	return ((size_t)ft_atoi(argv[1]));
}

void	get_away_corpses(size_t nb_philos, t_philo *cavern)
{
	size_t	corpses_cleaned;

	corpses_cleaned = 0;
	while (corpses_cleaned < nb_philos)
	{
		pthread_join(cavern[corpses_cleaned++].philo, NULL);
	}
	free(cavern);
}

void	chain_philos(size_t nb_philos, t_philo *cavern, int argc, char **argv)
{
	struct timeval	temps;
	size_t			cuffs;

	cuffs = 0;
	gettimeofday(&temps, NULL);
	while (cuffs < nb_philos)
	{
		if (!welcome_philos(&cavern[cuffs], cuffs + 1, argv, argc))
			break ;
		if (cuffs < nb_philos - 1 && nb_philos > 1)
			cavern[cuffs].nextfork = &cavern[cuffs + 1].fork;
		else
			cavern[cuffs].nextfork = &cavern[0].fork;
		cavern[cuffs].birth = temps;
		pthread_create(&cavern[cuffs].philo, NULL, &routine, &cavern[cuffs]);
		cuffs++;
	}
}

int	main(int argc, char **argv)
{
	struct timeval	temps;
	suseconds_t		times[2];
	t_philo			*cavern;
	size_t			nb_philos;

	nb_philos = ft_atoi(argv[1]);
	gettimeofday(&temps, NULL);
	times[0] = temps.tv_sec * 1000 + temps.tv_usec / 1000;
	cavern = malloc(sizeof(t_philo) * nb_philos);
	chain_philos(nb_philos, cavern, argc, argv);
	get_away_corpses(nb_philos, cavern);
	/*gettimeofday(&temps, NULL);
	times[1] = temps.tv_sec * 1000 + temps.tv_usec / 1000;
	printf("Time elapsed = %ums\n", times[1] - times[0]);*/
	return (0);
}
