/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_spe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:29:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/29 22:45:58 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	wait_l(t_tree *start)
{
	t_minishell	*p;
	int			status;

	p = start->first_elem;
	while (p->next != NULL)
	{
		if (waitpid(p->pid, NULL, WUNTRACED) == -1)
			return (-1);
		p = p->next;
	}
	if (p->next != NULL || waitpid(p->pid, &status, 0) == -1)
		return (-1);
	if ((WIFEXITED(status)) && WEXITSTATUS(status) != 0)
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (create_signal(), init_signal(0), sig_exit(WTERMSIG(status)));
	return (0);
}

int	first_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (!elem || elem == NULL || !p->env || pipe(elem->fd) == -1)
		return (-1);
	if (stop_signals() == 1)
		return (134);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (create_signal_spe() == -1 || g_sig.sig_int > 0
			|| g_sig.sig_quit > 0 || dup2(elem->fd[1], STDOUT_FILENO) < 0)
			abort_pipe(elem, p);
		if (close(elem->fd[0]) == -1 || close (elem->fd[1]) == -1
			|| cond_redir(elem, p) == -1
			|| exec_command(maj_path(p->env), elem->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	if (elem->next->next == NULL)
		return (create_signal(), init_signal(0), last_pipe_cat(elem, p, start));
	return (create_signal(), init_signal(0), mid_pipe_cat(elem, p, start));
}

int	end_pipe2_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	elem = elem->next;
	if (elem->next->next == NULL)
		return (create_signal(), init_signal(0), last_pipe_cat(elem, p, start));
	return (create_signal(), init_signal(0), mid_pipe_cat(elem, p, start));
}

int	mid_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (pipe(elem->next->fd) == -1)
		return (-1);
	if (stop_signals() == 1)
		return (134);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (create_signal_spe() == -1)
			abort_pipe2(elem, p);
		if (close(elem->fd[1]) == -1 || g_sig.sig_int > 0 || g_sig.sig_quit > 0
			|| (dup2(elem->fd[0], STDIN_FILENO) < 0
				|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0))
			abort_pipe3(elem, p);
		if (close(elem->next->fd[0]) == -1 || close(elem->fd[0]) == -1
			|| close(elem->next->fd[1]) == -1 || cond_redir(elem, p) == -1
			|| exec_command(maj_path(p->env), elem->next->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	if (close(elem->fd[0]) == -1 || close(elem->fd[1]) == -1)
		return (1);
	return (end_pipe2_cat(elem, p, start));
}

int	last_pipe_cat(t_minishell *e, t_all *p, t_tree *start)
{
	int	ret;

	if (stop_signals() == 1)
		return (134);
	e->next->pid = fork();
	if (e->next->pid < 0)
		return (-1);
	if (e->next->pid == 0)
	{
		if (create_signal_spe() == -1)
			abort_pipe(e, p);
		close(e->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0
			|| dup2(e->fd[0], STDIN_FILENO) < 0)
		{
			close(e->fd[0]);
			end_process(p, 1);
		}
		close(e->fd[0]);
		if (cond_redir(e, p) == -1
			|| exec_command(maj_path(p->env), e->next->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	if (close(e->fd[0]) == -1 || close(e->fd[1]) == -1)
		return (create_signal(), init_signal(0), -1);
	ret = wait_l(start);
	return (create_signal(), init_signal(0), ret);
}
