/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:23:26 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/19 16:24:01 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	full_digit(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (false);
	return (true);
}

void	ft_exit(t_all *p, t_tree *start, char **cmd)
{
	int	nb;
	
	if (length_tab(cmd) > 1 && full_digit(cmd[1]) && cmd[2])
		return (ft_putendl_fd("exit: too many arguments", 2));
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
	printf("exit\n");
	free(g_sig.line);
	free_start(start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	rl_clear_history();
	//get_next_line(-42);
	exit(nb);
}
