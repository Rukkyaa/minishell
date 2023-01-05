/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/05 14:15:44 by gduhau           ###   ########.fr       */
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

int	first_pipe(t_minishell *elem, char **paths, char **env)
{
	if (!elem || elem == NULL || !paths || !env || pipe(elem->fd) == -1)
		return (-1);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->infile != NULL && opening(elem->infile, STDIN_FILENO, 0, 0) == -1)
			exit (42);
		close(elem->fd[0]);
		if (elem->outfile != NULL && opening(elem->outfile, STDOUT_FILENO, elem->append, 1) == -1)
			exit (42);
		if (dup2(elem->fd[1], STDOUT_FILENO) < 0)
			exit(42);
		if (exec_command(paths, elem->cmd, env) == -1)
			exit(42);
		exit(0);
	}
	if (elem->next->next == NULL)
		return (last_pipe(elem, paths, env));
	return (mid_pipe(elem, paths, env));
}

int	mid_pipe(t_minishell *elem, char **paths, char **env)
{
	int	status1;

	if (pipe(elem->next->fd) == -1)
		return (-1);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (elem->next->infile != NULL && opening(elem->next->infile, STDIN_FILENO, 0, 0) == -1)
			exit (42);
		if (elem->next->outfile != NULL && opening(elem->next->outfile, STDOUT_FILENO, elem->next->append, 1) == -1)
			exit (42);
		close(elem->fd[1]);
		if (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0)
			exit(42);
		close(elem->next->fd[0]);
		if (exec_command(paths, elem->next->cmd, env) == -1)
			exit (42);
		exit(0);
	}
	if ((close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1)
		|| (WIFEXITED(status1) && WEXITSTATUS(status1) == 42))
		return (-1);
	elem = elem->next;
	if (elem->next->next == NULL)
		return (last_pipe(elem, paths, env));
	return (mid_pipe(elem, paths, env));
}


int	last_pipe(t_minishell *elem, char **paths, char **env)
{
	int	status1;
	int	status2;

	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (elem->next->infile != NULL && opening(elem->next->infile, STDIN_FILENO, 0, 0) == -1)
			exit (42);
		if (elem->next->outfile != NULL && opening(elem->next->outfile, STDOUT_FILENO, elem->next->append, 1) == -1)
			exit (42);
		close(elem->fd[1]);
		if (dup2(elem->fd[0], STDIN_FILENO) < 0)
			exit(42);
		if (exec_command(paths, elem->next->cmd, env) == -1)
			exit(42);
		exit(0);
	}
	if (close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1
		|| waitpid(elem->next->pid, &status2, 0) == -1
		|| ((WIFEXITED(status1) && WEXITSTATUS(status1) == 42)
			|| (WIFEXITED(status2) && WEXITSTATUS(status2) == 42)))
		return (-1);
	return (0);
}
