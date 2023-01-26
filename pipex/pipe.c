/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 13:41:21 by gatsby           ###   ########.fr       */
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

void	abort_pipe(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
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

void	abort_pipe2(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->fd[1]) == -1
		|| close(elem->next->fd[0]) == -1 || close(elem->next->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}

void	abort_pipe3(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->next->fd[0]) == -1
		|| close(elem->next->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}

int	end_pipe2(t_minishell *elem, t_all *p, t_tree *start)
{
	if (close_all(elem->fd[0], elem->fd[1], NULL, elem->pid) == -1)
		return (create_signal(), init_signal(0), -1);
	elem = elem->next;
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

static int	end_pipe3(t_minishell *elem)
{
	int	status2;

	close(elem->fd[0]);
	close(elem->fd[1]);
	if (waitpid(elem->pid, NULL, 0) == -1)
		return (create_signal(), init_signal(0), -1);
	if (waitpid(elem->next->pid, &status2, 0) == -1
		|| ((WIFEXITED(status2)) && WEXITSTATUS(status2) != 0))
		return (create_signal(), init_signal(0), WEXITSTATUS(status2));
	return (create_signal(), init_signal(0), 0);
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
