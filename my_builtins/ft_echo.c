/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:53:56 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/30 23:59:38 by gatsby           ###   ########.fr       */
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

static int	conditions_echo(char *cmd)
{
	if (!(ft_strlen(cmd) == 2
			&& ((cmd[0] == '\"' && cmd[1] == '\"')
				|| (cmd[0] == '\'' && cmd[1] == '\''))))
		return (1);
	return (0);
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
	if (write(1, "\0", 1) == -1)
		if (errno == ENOSPC)
			return (ft_putendl_fd(
					"echo: write error: No space left on device", 2), 1);
	if (cmd[1])
	{
		while (cmd[++i] && check_echo(cmd[i]))
			option = true;
		while (cmd[i])
		{
			if (conditions_echo(cmd[i]) == 1)
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
