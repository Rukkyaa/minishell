/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:50:09 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/18 19:58:24 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	unsigned int	result;
	unsigned int	index;
	int				sign;

	result = 0;
	index = 0;
	sign = 1;
	if (nptr == NULL)
		return (0);
	while (nptr[index] == ' ' || (nptr[index] >= 9 && nptr[index] <= 13))
		index ++;
	if (nptr[index] == '+' || nptr[index] == '-')
	{
		if (nptr[index] == '-')
			sign = -1;
		index ++;
	}
	while (nptr[index] >= '0' && nptr[index] <= '9')
	{
		result *= 10;
		result += nptr[index] - '0';
		index ++;
	}
	return (sign * result);
}
