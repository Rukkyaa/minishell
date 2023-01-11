/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:42:24 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/11 16:17:45 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int echo_comp(char **cmd)
{
	if (ft_strcmp(cmd[0], "echo") == 0)
	{
		if (ft_strcmp(cmd[1], "-n") == 0)
		{
			if (cmd[2] == NULL)
				return (0);
		}
		else if (cmd[1] == 0)
			return (0);
	}
	return (-1);
}

int path_comp_builtins(char **cmd) //check si il n'y a pas de leak avec le second membre des strcmp
{
	if (echo_comp(cmd) == 0)
		return (1);
	// else if (ft_strcmp(cmd[0], "cd") == 0)
	// 	return (2);
	else if (ft_strcmp(cmd[0], "pwd") == 0 && cmd[1] == NULL)
		return (3);
	// else if (ft_strcmp(cmd[0], "export") == 0 && cmd[1] == NULL)
	// 	return (4);
	// else if (ft_strcmp(cmd[0], "unset") == 0 && cmd[1] == NULL)
	// 	return (5);
	// else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
	// 	return (6);
	// else if (ft_strcmp(cmd[0], "exit") == 0 && cmd[1] == NULL)
	// 	return (7);
	return (0);
}

void exec_builtin(int nb, char **cmd)
{
	(void) cmd;
	if (nb == 1)
		return ;//ft_echo(cmd);
	else if (nb == 3)
		ft_pwd();
	return ;
}