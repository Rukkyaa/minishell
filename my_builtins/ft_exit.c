/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:23:26 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/30 10:42:11 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	full_digit(char *str)
{
	int	i;

	i = -1;
	if (ft_strlen(str) > 1 && (str[0] == '-' || str[0] == '+'))
		++i;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
	}
	return (true);
}

void	free_exit(t_all *p, t_tree *start)
{
	free(g_sig.line);
	free_start(start, 0);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	rl_clear_history();
}

void	ft_exit(t_all *p, t_tree *start, char **cmd)
{
	int	nb;

	if (length_tab(cmd) > 1 && full_digit(cmd[1]) && cmd[2])
		return (ft_putstr_fd("exit\n", 2),
			ft_putendl_fd("exit: too many arguments", 2));
	printf("exit\n");
	if (cmd[1])
	{
		if (!full_digit(cmd[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			nb = 2;
		}
		else
			nb = (unsigned char)ft_atoi(cmd[1]);
	}
	else
		nb = 0;
	free_exit(p, start);
	exit(nb);
}
