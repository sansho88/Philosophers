/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isdigit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 12:00:38 by tgriffit          #+#    #+#             */
/*   Updated: 2022/04/12 15:05:44 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/**	Tests for a decimal digit character.
 * @param size = amount of "words"
 * */
int	ft_str_isdigit(char **str, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < size)
	{
		if ((str[i][j] == '-' && !ft_isdigit((str)[i][j + 1])) || (!ft_isdigit
		(str[i][j]) && str[i][j] != '-'))
			return (0);
		j++;
		if (!str[i][j])
		{
			i++;
			j = 0;
		}
	}
	return (1);
}
