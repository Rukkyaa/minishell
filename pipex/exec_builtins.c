/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:42:24 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/16 17:10:54 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int echo_comp(char **cmd)
// {
// 	if (ft_strcmp(cmd[0], "echo") == 0)
// 	{
// 		if (ft_strcmp(cmd[1], "-n") == 0)
// 		{
// 			if (cmd[2] == NULL)
// 				return (0);
// 		}
// 		else if (cmd[1] != NULL)
// 			return (0);
// 	}
// 	return (-1);
// }

int path_comp_builtins(char **cmd) //check si il n'y a pas de leak avec le second membre des strcmp
{
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "cd") == 0) // a reprendre
		return (-2);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd[0], "export") == 0) //a reprendre
		return (-4);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (-5);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		return (6);
	else if (ft_strcmp(cmd[0], "exit") == 0 && cmd[1] == NULL)
		return (7);
	return (0);
}

int exec_builtin(int nb, char **cmd, t_all *p, t_tree *start)
{
	if (nb == 1)
		return (ft_echo(cmd));
	else if (nb == -2)
		return (ft_cd(p->env, cmd)); //reprendre cd
	else if (nb == 3)
	{
		printf("PWD !\n");
		return (ft_pwd());
	}
	else if (nb == -4)
		return (ft_export(p->env, cmd));
	else if (nb == -5)
		return (ft_unset(p->env, cmd));
	else if (nb == 6)
		return (ft_env(p->env));
	else if (nb == 7)
		ft_exit(p, start);
	return (-1);
}