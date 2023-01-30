/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:46:20 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/30 12:48:38 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	end_process(t_all *p, int nb)
{
	free_start(p->start, 0);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	free(g_sig.line);
	rl_clear_history();
	if (g_sig.sig_quit == 1)
		exit(134);
	else if (g_sig.sig_int == 1)
		exit(0);
	else if (g_sig.cmd_stat == 127)
		exit (127);
	else if (g_sig.cmd_stat == 126)
		exit (126);
	exit(nb);
}

void	clean_p(char **paths, int flag, t_all *p)
{
	if (paths != NULL)
		free_tab(paths);
	if (flag == 1)
	{
		ft_putstr_fd("'' : command not found\n", 2);
		end_process(p, 0);
	}
}

void	emergency(t_minishell *e, t_all *p)
{
	if (close(e->fd[0]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}
