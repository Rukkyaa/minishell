/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:53:56 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/23 09:13:30 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char *str)
{
	char	**split;
	int		i;
	bool	option;

	split = ft_split(str + 4, ' ');
	if (!split)
		return (EXIT_FAILURE);
	option = false;
	i = -1;
	if (split[0])
	{
		while (!strcmp(split[++i], "-n"))
			option = true;
		while (split[i])
		{
			ft_putstr_fd(split[i], 1);
			if (split[i][0] && split[i + 1])
				ft_putchar_fd(' ', 1);
			i ++;
		}
	}
	if (!option)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
