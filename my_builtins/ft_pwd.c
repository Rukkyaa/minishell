/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 08:37:21 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/19 17:08:06 by gatsby           ###   ########.fr       */
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
		return (0);
	}
	return (1);
}
