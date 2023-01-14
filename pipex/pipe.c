/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/13 15:32:06 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	close_all(int fd1, int fd2, int *status1, pid_t pid)
{
	close(fd1);
	close(fd2);
	if (waitpid(pid, status1, 0) < -1)
		return (-1);
	return (0);
}

int	kill1(t_minishell *elem, int stat)
{
	close(elem->next->fd[0]);
	close(elem->next->fd[1]);
	kill(elem->next->pid, SIGINT);
	if (waitpid(elem->next->pid, NULL, 0) < -1)
		return (stat);
	return (stat);
}

int	first_pipe(t_minishell *elem, t_all *p)
{
	if (!elem || elem == NULL || !p->paths || !p->env || pipe(elem->fd) == -1)
		return (-1);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (signal(SIGINT, &sig_int) == SIG_ERR || signal(SIGQUIT, &sig_quit) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int != 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
			error_process(p);
		close(elem->fd[0]);
		if (g_sig.sig_int != 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || dup2(elem->fd[1], STDOUT_FILENO) < 0)
			error_process(p);
		if (g_sig.sig_int != 0 || exec_command(p->paths, elem->cmd, p) == -1)
			error_process(p);
		exit(0);
	}
	if (elem->next->next == NULL)
		return (last_pipe(elem, p));
	return (mid_pipe(elem, p));
}

int	mid_pipe(t_minishell *elem, t_all *p)
{
	int	status1;

	if (pipe(elem->next->fd) == -1)
		return (-1);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (signal(SIGINT, &sig_int) == SIG_ERR || signal(SIGQUIT, &sig_quit) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int != 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		close(elem->fd[1]);
		if (g_sig.sig_int != 0 || (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0))
			error_process(p);
		close(elem->next->fd[0]);
		if (g_sig.sig_int != 0 || exec_command(p->paths, elem->next->cmd, p) == -1)
			error_process(p);
		exit(0);
	}
	if ((close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1)
		|| (WIFEXITED(status1) && WEXITSTATUS(status1) != 0))
		return (kill1(elem, WEXITSTATUS(status1)));
	elem = elem->next;
	if (elem->next->next == NULL)
		return (last_pipe(elem, p));
	return (mid_pipe(elem, p));
}


int	last_pipe(t_minishell *elem, t_all *p)
{
	int	status1;
	int	status2;

	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (signal(SIGINT, &sig_int) == SIG_ERR || signal(SIGQUIT, &sig_quit) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int != 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		close(elem->fd[1]);
		if (g_sig.sig_int != 0 || dup2(elem->fd[0], STDIN_FILENO) < 0)
			error_process(p);
		if (g_sig.sig_int != 0 || exec_command(p->paths, elem->next->cmd, p) == -1)
			error_process(p);
		exit(0);
	}
	close(elem->fd[0]);
	close(elem->fd[1]);
	if (waitpid(elem->pid, &status1, 0) < -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (kill1(elem, WEXITSTATUS(status1)));
	close(elem->next->fd[0]);
	close(elem->next->fd[1]);
	if (waitpid(elem->pid, &status2, 0) < -1 || ((WIFEXITED(status2)) && WEXITSTATUS(status2) != 0))
		return (WEXITSTATUS(status2));
	// if (close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1
	// 	|| waitpid(elem->next->pid, &status2, 0) == -1
	// 	|| ((WIFEXITED(status1) && WEXITSTATUS(status1) != 0)
	// 		|| (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)))
	// 	return (kill2(elem, p, WEXITSTATUS(status1), WEXITSTATUS(status2)));
	return (0);
}
