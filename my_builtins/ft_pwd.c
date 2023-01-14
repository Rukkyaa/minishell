/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:37:21 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/14 10:56:56 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_cmd(char *cmd)
{
	int	i;

	i = 0;
	if (cmd == NULL)
		return (0);
	while (cmd[i] != '\0' && (cmd[i] == '\'' || cmd[i] == '\"'))
		i++;
	if (cmd[i] == '-')
		return (1);
	return (0);
}

int	check_options(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		if (check_cmd(cmd[i++]) == 1)
			return (1);
	}
	return (0);
}

int	ft_pwd(void)
{
	char	buffer[PATH_MAX];

	if (getcwd(buffer, PATH_MAX))
	{
		ft_putendl_fd(buffer, 1);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
