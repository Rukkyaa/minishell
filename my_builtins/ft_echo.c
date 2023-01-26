/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:53:56 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/26 17:19:23 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Function to check if the string is a valid echo option
** to allow for multiple n's in the option
**
** @param str: string to check
**
** @return: true if valid, false if not
*/
bool	check_echo(char *str)
{
	unsigned int	i;

	if (ft_strlen(str) < 2)
		return (false);
	else if (str[0] != '-')
		return (false);
	i = 0;
	while (str[++i])
		if (str[i] != 'n')
			return (false);
	return (true);
}

/*
** Our own echo function
** Usage : echo [option] [string]
** Options : -n
** -n : do not output the trailing newline
**
** @param cmd: array of strings containing the command and its arguments
**
** @return: EXIT_SUCCESS (0)
*/
int	ft_echo(char **cmd)
{
	int		i;
	bool	option;

	option = false;
	i = 0;
	if (cmd[1])
	{
		while (cmd[++i] && check_echo(cmd[i]))
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
	return (EXIT_SUCCESS);
}
