/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:53:56 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/19 17:07:38 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **cmd)
{
	int		i;
	bool	option;

	option = false;
	i = 0;
	if (cmd[1])
	{
		while (cmd[++i] && !ft_strncmp(cmd[i], "-n\0", 3))
			option = true;
		while (cmd[i])
		{
			ft_putstr_fd(cmd[i], 1);
			if (cmd[i][0] && cmd[i + 1])
				ft_putchar_fd(' ', 1);
			i ++;
		}
	}
	if (!option)
		ft_putchar_fd('\n', 1);
	return (0);
}
