/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/25 12:36:17 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	close_all(int fd1, int fd2, int *status1, pid_t pid)
{
	close(fd1);
	close(fd2);
	if (waitpid(pid, status1, 0) == -1)
		return (-1);
	return (0);
}

// int	kill1(t_minishell *elem, int stat, int opt)
// {
// 	if (opt == 2)
// 	{
// 		close(elem->next->fd[0]);
// 		close(elem->next->fd[1]);
// 	}
// 	kill(elem->next->pid, SIGINT);
// 	if (waitpid(elem->next->pid, NULL, 0) < -1)
// 		return (stat);
// 	return (stat);
// }

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
		if (create_signal_spe() == -1)
		{
			close (elem->fd[0]);
			close (elem->fd[1]);
			end_process(p, 1);
		}
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[1], STDOUT_FILENO) < 0)
		{
			close (elem->fd[0]);
			close (elem->fd[1]);
			end_process(p, 1);
		}
		close(elem->fd[0]);
		close (elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
		{
			//close (elem->fd[1]);
			end_process(p, 1);
		}
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
		{
			//close(elem->fd[1]);
			end_process(p, 1);
		}
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->cmd, p, start) != 0)
		{
			//close (elem->fd[1]);
			end_process(p, 1);
		}
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
		{
			close(elem->fd[0]);
			close(elem->fd[1]);
			close(elem->next->fd[0]);
			close(elem->next->fd[1]);
			end_process(p, 1);
		}
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0))
		{
			close(elem->fd[0]);
			close(elem->next->fd[0]);
			close(elem->next->fd[1]);
			end_process(p, 1);
		}
		close(elem->next->fd[0]);
		close(elem->fd[0]);
		close(elem->next->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
		{
			end_process(p, 1);
		}
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
		{
			end_process(p, 1);
		}
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->next->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	// if ((close_all(elem->fd[0], elem->fd[1], &status1, elem->pid) == -1)
	// 	|| (WIFEXITED(status1) && WEXITSTATUS(status1) != 0))
	// 	return (create_signal(), init_signal(0), kill1(elem, WEXITSTATUS(status1), 2));
	if (close_all(elem->fd[0], elem->fd[1], NULL, elem->pid) == -1)
		return (create_signal(), init_signal(0), -1);
	elem = elem->next;
	if (elem->next->next == NULL)
		return (create_signal(), init_signal(0), last_pipe(elem, p, start));
	return (create_signal(), init_signal(0), mid_pipe(elem, p, start));
}


int	last_pipe(t_minishell *elem, t_all *p, t_tree *start)
{
	//int	status1;
	int	status2;

	if (stop_signals() == 1)
		return (134);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (create_signal_spe() == -1)
		{
			close(elem->fd[0]);
			close(elem->fd[1]);
			end_process(p, 1);
		}
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[0], STDIN_FILENO) < 0)
		{
			close(elem->fd[0]);
			end_process(p, 1);
		}
		close(elem->fd[0]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->next->cmd, p, start) != 0)
			end_process(p, 1);
		exit(0);
	}
	close(elem->fd[0]);
	close(elem->fd[1]);
	if (waitpid(elem->pid, NULL, 0) == -1) //|| ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (create_signal(), init_signal(0), -1); //kill1(elem, WEXITSTATUS(status1), 3));
	if (waitpid(elem->next->pid, &status2, 0) == -1 || ((WIFEXITED(status2)) && WEXITSTATUS(status2) != 0))
		return (create_signal(), init_signal(0), WEXITSTATUS(status2));
	return (create_signal(), init_signal(0), 0);
}
