/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:02:21 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/26 22:56:52 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_command_one(t_minishell *elem, t_all *p, t_tree *start)
{
	int	status1;

	if (path_comp_builtins(elem->cmd) < 0)
		return (exec_builtin(path_comp_builtins(elem->cmd),
				elem->cmd, p, start));
	if (stop_signals() == 1)
		return (134);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (create_signal_spe() == -1 || cond_redir(elem, p) == -1)
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0
			|| exec_command(maj_path(p->env), elem->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) == -1
		|| ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (create_signal(), init_signal(0), WEXITSTATUS(status1));
	return (create_signal(), init_signal(0), 0);
}

int	pipex(t_minishell *first_elem, t_all *p, t_tree *start)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, p, start));
	else if (first_elem->next != NULL && first_elem->next->next != NULL
		&& find_cat(first_elem) == 1)
		return (first_pipe_cat(first_elem, p, start));
	return (first_pipe_cat(first_elem, p, start));
}

int	find_cat(t_minishell *elem)
{
	t_minishell	*p;

	p = elem;
	while (p != NULL)
	{
		if (ft_strcmp(p->cmd[0], "cat") == 0 && p->cmd[1] == NULL
			&& p->file_in == NULL && p->file_out == NULL)
			return (1);
		p = p->next;
	}
	return (0);
}

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
