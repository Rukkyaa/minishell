/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 15:43:44 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/26 15:45:47 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	close_all(int fd1, int fd2, int *status1, pid_t pid)
{
	if (close(fd1) == -1 || close(fd2) == -1)
		return (-1);
	if (waitpid(pid, status1, 0) == -1)
		return (-1);
	return (0);
}

int	first_pipe(t_minishell *elem, t_all *p, t_tree *start)
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
		return (create_signal(), init_signal(0), last_pipe(elem, p, start));
	return (create_signal(), init_signal(0), mid_pipe(elem, p, start));
}

int	mid_pipe(t_minishell *elem, t_all *p, t_tree *start)
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
	return (end_pipe2(elem, p, start));
}

int	last_pipe(t_minishell *elem, t_all *p, t_tree *start)
{
	if (stop_signals() == 1)
		return (134);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (create_signal_spe() == -1)
			abort_pipe(elem, p);
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0
			|| dup2(elem->fd[0], STDIN_FILENO) < 0)
		{
			close(elem->fd[0]);
			end_process(p, 1);
		}
		close(elem->fd[0]);
		if (cond_redir(elem, p) == -1
			|| exec_command(maj_path(p->env), elem->next->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	return (end_pipe3(elem));
}
