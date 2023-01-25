/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_spe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:29:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/25 17:47:13 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	wait_all(t_tree *start)
{
	t_minishell *p;
	int	status;

	p = start->first_elem;
	while (p->next != NULL)
	{
		close(p->fd[0]);
		close(p->fd[1]);
		p = p->next;
	}
	p = start->first_elem;
	while (p != NULL)
	{
		if (waitpid(p->pid, &status, 0) == -1 || ((WIFEXITED(status)) && WEXITSTATUS(status) != 0)) //gerer le cas d'erreur
			return (-1);
		p = p->next;
	}
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
		if (create_signal_spe() == -1)
			end_process(p, 1);
		close(elem->fd[0]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[1], STDOUT_FILENO) < 0)
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->cmd, p, start) == -1)
			end_process(p, 1);
		exit(0);
	}
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
			end_process(p, 1);
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0))
			end_process(p, 1);
		close(elem->next->fd[0]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->next->cmd, p, start) == -1)
			end_process(p, 1);
		exit(0);
	}
	elem = elem->next;
	if (elem->next->next == NULL)
		return (create_signal(), init_signal(0), last_pipe_cat(elem, p, start));
	return (create_signal(), init_signal(0), mid_pipe_cat(elem, p, start));
}

int	last_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (stop_signals() == 1)
		return (134);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (create_signal_spe() == -1)
			end_process(p, 1);
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[0], STDIN_FILENO) < 0)
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO, elem->cmd, p) == -1))
			end_process(p, 1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(maj_path(p->env), elem->next->cmd, p, start) == -1)
			end_process(p, 1);
		exit(0);
	}
	if (wait_all(start) == -1)
		return(create_signal(), init_signal(0), -1); //gestion d'erreur avec les kill
	return (create_signal(), init_signal(0), 0);
}