/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 12:03:55 by tgriffit          #+#    #+#             */
/*   Updated: 2022/04/12 14:42:15 by tgriffit         ###   ########.fr       */
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
	pthread_mutex_t	block;

	gettimeofday(&temps, NULL);
	start = temps.tv_usec;
	while (temps.tv_usec - start < time) //TODO: YA LIDEE WSH
		{
	//	printf("[%s]temps.tv_usec - start = %d - %d = %d\n", __func__, temps.tv_usec, start, temps.tv_usec - start);
			gettimeofday(&temps, NULL);
		}
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

void	parse_args(int argc, char **argv)
{
	if (argc < 4 || argc > 5)
	{
		puts("Correct usage: ./philo [number_of_philosophers] [time_to_die] "
			"[time_to_eat] [time_to_sleep] "
			"(number_of_times_each_philosopher_must_eat)");
		return ;
	}
	/*number_of_philosophers time_to_die time_to_eat
	time_to_sleep
	[number_of_times_each_philosopher_must_eat]*/
}

void	welcome_philos_in_cavern(t_philo *philo, size_t id, char **argv)
{
	philo->id = id;
	philo->starve = 10;
	philo->fork.isfree = true;
	philo->isalive = true;
	gettimeofday(&philo->lastmeal, NULL);
}

int	main(int argc, char **argv)
{
	struct timeval	temps;
	suseconds_t		start;
	suseconds_t		end;
	t_philo			*cavern;
	size_t			i;
	size_t			nb_philos;

	nb_philos = 100;
	gettimeofday(&temps, NULL);
	start = temps.tv_sec * 1000 + temps.tv_usec / 1000;
	cavern = malloc(sizeof(t_philo) * nb_philos);
	i = 0;
	while (i < nb_philos)
	{
		welcome_philos_in_cavern(&cavern[i], i + 1, argv);
		if (i < nb_philos - 1 && nb_philos > 1)
			cavern[i].nextfork = &cavern[i + 1].fork;
		else
			cavern[i].nextfork = &cavern[0].fork;
		cavern[i].birth = temps;
		pthread_create(&cavern[i].philo, NULL, &routine, &cavern[i]);
		i++;
	}
	i = 0;
	while (i < nb_philos)
		pthread_join(cavern[i++].philo, NULL);
	gettimeofday(&temps, NULL);
	end = temps.tv_sec * 1000 + temps.tv_usec / 1000;
	printf("Time elapsed = %u\nstart = %u, end = %u\n", end - start, start, end);
	return (0);
}
