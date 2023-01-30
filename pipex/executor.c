/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/30 10:32:29 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_size_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		++i;
	}
	return (i);
}

char	**env_to_char(t_env *env)
{
	char	**reforged;
	int		i;
	int		e;

	i = 0;
	e = 0;
	if (!env || env == NULL)
		return (NULL);
	i = ft_size_env(env);
	reforged = malloc((i + 1) * sizeof(char *));
	if (!reforged)
		return (NULL);
	while (e < i)
	{
		if (env->code == 1 || env->code == 2 || env->code == 4)
		{
			reforged[e] = ft_strjoin(env->key, "=");
			reforged[e] = ft_strjoin_spe(reforged[e], env->value);
			if (reforged[e++] == NULL)
				return (free_tab(reforged), NULL);
		}
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

int	stop_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR
		|| signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		return (1);
	return (0);
}

int	cond_redir(t_minishell *elem, t_all *p)
{
	if (elem->cmd == NULL
		&& ((elem->file_in != NULL && elem->file_in->file_in == NULL)
			|| (elem->file_out != NULL && elem->file_out->file_out == NULL)))
	{
		printf(" : No such file or directory\n");
		end_process(p, 0);
	}
	if (g_sig.sig_int > 0 || g_sig.sig_quit > 0
		|| (elem->file_in != NULL
			&& opening_in(elem->file_in, STDIN_FILENO, elem->cmd, p) == -1)
		|| (elem->file_out != NULL
			&& opening_out(elem->file_out, STDOUT_FILENO, elem->cmd, p) == -1))
		return (-1);
	return (0);
}

int	executor(t_tree *start, t_all *p, char *line)
{
	t_tree	*intermed;

	if (start == NULL || !start)
		return (-1);
	g_sig.cmd_stat = pipex(start->first_elem, p, start);
	init_signal(0);
	if (g_sig.cmd_stat != 0)
	{
		if (start->or && start->or != NULL)
		{
			free(start->cmd);
			free_minishell(start->first_elem, 0);
			return (intermed = start->or, free(start),
				p->start = intermed, executor(p->start, p, line));
		}
		return (free_start(start, 0), g_sig.cmd_stat);
	}
	if (start->and && start->and != NULL)
	{
		free(start->cmd);
		free_minishell(start->first_elem, 0);
		return (intermed = start->and, free(start),
			p->start = intermed, executor(p->start, p, line));
	}
	return (free_start(start, 0), g_sig.cmd_stat);
}
