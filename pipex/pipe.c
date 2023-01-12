/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/12 11:27:39 by gduhau           ###   ########.fr       */
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

int	first_pipe(t_minishell *elem, t_all *p)
{
	if (!elem || elem == NULL || !p->paths || !p->env || pipe(elem->fd) == -1)
		return (-1);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1)
			error_process(p);
		close(elem->fd[0]);
		if (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1)
			error_process(p);
		if (dup2(elem->fd[1], STDOUT_FILENO) < 0)
			error_process(p);
		if (exec_command(p->paths, elem->cmd, p) == -1)
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
		if (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1)
			error_process(p);
		if (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1)
			error_process(p);
		close(elem->fd[1]);
		if (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0)
			error_process(p);
		close(elem->next->fd[0]);
		if (exec_command(p->paths, elem->next->cmd, p) == -1)
			error_process(p);
		exit(0);
	}
	if ((close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1)
		|| (WIFEXITED(status1) && WEXITSTATUS(status1) == 1))
		return (-1);
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
		if (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1)
			error_process(p);
		if (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1)
			error_process(p);
		close(elem->fd[1]);
		if (dup2(elem->fd[0], STDIN_FILENO) < 0)
			error_process(p);
		if (exec_command(p->paths, elem->next->cmd, p) == -1)
			error_process(p);
		exit(0);
	}
	if (close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1
		|| waitpid(elem->next->pid, &status2, 0) == -1
		|| ((WIFEXITED(status1) && WEXITSTATUS(status1) == 1)
			|| (WIFEXITED(status2) && WEXITSTATUS(status2) == 1)))
		return (-1);
	return (0);
}
