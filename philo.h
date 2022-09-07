/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:40:08 by tgriffit          #+#    #+#             */
/*   Updated: 2022/08/25 15:57:16 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>

# define MALLOC_ERR 42

typedef struct s_corpse{
	struct s_philo	*dead_philo;
	long			time_of_death;
}	t_corpse;

typedef struct s_world{
	size_t			nb_philos;
	size_t			nb_meals_max;
	pthread_mutex_t	check_go;
	pthread_mutex_t	god_voice;
	bool			go;
	struct timeval	start;
	int				argc;
	char			**argv;
	suseconds_t		lifetime;
	suseconds_t		time4eat;
	suseconds_t		sleeptime;
	struct s_philo	*cavern;
	t_corpse		*dead_philo;
}	t_world;

typedef struct s_philo{
	t_world			*world;
	pthread_t		philo;
	size_t			nb_meals;
	unsigned int	id;
	struct timeval	lastmeal;
	pthread_mutex_t	fork;
	pthread_mutex_t	*nextfork;
	pthread_mutex_t	has_eaten;
	suseconds_t		lifetime;
	suseconds_t		time4eat;
	suseconds_t		sleeptime;
}	t_philo;

// UTILS
int				ft_str_isdigit(char **str, size_t size);
int				ft_isdigit(int c);
int				ft_atoi(const char *str);
unsigned int	ft_strlen(const char *str);
char			*ft_strchr(const char *s, int c);

// CHRONOS
long			get_timestamp(t_world world);
long			ft_timer_since(struct timeval start);
void			myusleep(useconds_t time);

// NURSERY
void			init_world(t_world *world, int argc, char **argv);
bool			give_birth_to_philo(t_philo *philo, t_world *world);
bool			place_philos_in_cavern(t_philo *cavern, t_world *world);
void			light_on_cavern(t_philo *cavern, t_world *world,
					size_t nb_philos);

// AMANUENSIS (Secretary)
bool			try_to_use(pthread_mutex_t *mutex, const bool *value_to_check);
void			print_act(char *msg, t_philo philo, t_world *world);
bool			check_args(int argc, char **argv);
bool			check_arg_value(const char *argv);

// DAY & NIGHT
void			*routine(void	*philosoph);

// ANIHILATION (THE END)
void			unchain_philos(t_philo *cavern);
void			put_in_coffin(t_corpse *corpse, t_philo *philo, t_world *world);
void			announce_death_to_family(t_corpse corpse, t_world *world);

void			print_act(char *msg, t_philo philo, t_world *world);
#endif
