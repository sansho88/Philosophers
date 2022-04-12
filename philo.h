/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:17:48 by tgriffit          #+#    #+#             */
/*   Updated: 2022/04/12 14:01:30 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_fork{
	bool	isfree;
}	t_fork;

typedef struct s_philo{
	pthread_t		philo;
	size_t			id;
	suseconds_t		lifetime;
	suseconds_t		resttime;
	suseconds_t		starve; //Speed for eat
	bool			isalive;
	struct timeval	birth;
	struct timeval	lastmeal;
	struct s_fork	fork;
	struct s_fork	*nextfork;
}	t_philo;



#endif
