/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:42:24 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/27 10:29:09 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	path_comp_builtins(char **cmd)
{
	if (cmd == NULL || cmd[0] == NULL)
		return (0);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (-2);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (-4);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (-5);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		return (6);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (-7);
	return (0);
}

int	exec_builtin(int nb, char **cmd, t_all *p, t_tree *start)
{
	if (nb == 1)
		end_process(p, ft_echo(cmd));
	else if (nb == -2)
		return (ft_cd(p->env, cmd));
	else if (nb == 3)
		end_process(p, ft_pwd());
	else if (nb == -4)
		return (ft_export(p->env, cmd));
	else if (nb == -5)
		return (ft_unset(p->env, cmd));
	else if (nb == 6)
		end_process(p, ft_env(p->env));
	else if (nb == -7)
		return (ft_exit(p, start, cmd), 1);
	end_process(p, 1);
	return (1);
}
