/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriffit <tgriffit@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 18:37:01 by tgriffit          #+#    #+#             */
/*   Updated: 2022/09/08 14:31:43 by tgriffit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Converts the initial portion of the string pointed
 * to by str to int representation. Doesn't support multiples ' - '.
 * @param str = Number to convert to int
 * @return The string converted to an integer
 */
int	ft_atoi(const char *str)
{
	int	nb;
	int	signe;
	int	i;

	signe = 1;
	nb = 0;
	i = 0;
	while ((str[i] >= 8 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signe *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		nb = (nb * 10) + (str[i++] - '0');
	return (nb * signe);
}
