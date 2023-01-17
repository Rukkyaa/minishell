/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_spe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:29:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/17 18:17:48 by gabrielduha      ###   ########.fr       */
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
		if (waitpid(p->pid, &status, 0) < -1 || ((WIFEXITED(status)) && WEXITSTATUS(status) != 0)) //gerer le cas d'erreur
			return (-1);
		p = p->next;
	}
	return (0);
}

int	first_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (check_minishell(elem->cmd) == 1)
	{
		g_sig.sig_int = -1;
		g_sig.sig_quit = -1;
		p->env = shlvl(1, p->env);
	}
	if (!elem || elem == NULL || !p->paths || !p->env || pipe(elem->fd) == -1)
		return (-1);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (signal(SIGINT, &sighandler) == SIG_ERR || signal(SIGQUIT, &sig_eof) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
			error_process(p);
		close(elem->fd[0]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[1], STDOUT_FILENO) < 0)
			error_process(p);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(p->paths, elem->cmd, p, start) == -1)
			error_process(p);
		exit(0);
	}
	if (elem->next->next == NULL)
		return (last_pipe_cat(elem, p, start));
	return (mid_pipe_cat(elem, p, start));
}

int	mid_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (check_minishell(elem->next->cmd) == 1)
	{
		g_sig.sig_int = -1;
		g_sig.sig_quit = -1;
		p->env = shlvl(1, p->env);
	}
	if (pipe(elem->next->fd) == -1)
		return (-1);
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (signal(SIGINT, &sighandler) == SIG_ERR || signal(SIGQUIT, &sig_eof) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (dup2(elem->fd[0], STDIN_FILENO) < 0
			|| dup2(elem->next->fd[1], STDOUT_FILENO) < 0))
			error_process(p);
		close(elem->next->fd[0]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(p->paths, elem->next->cmd, p, start) == -1)
			error_process(p);
		exit(0);
	}
	elem = elem->next;
	if (elem->next->next == NULL)
		return (last_pipe_cat(elem, p, start));
	return (mid_pipe_cat(elem, p, start));
}

int	last_pipe_cat(t_minishell *elem, t_all *p, t_tree *start)
{
	if (check_minishell(elem->next->cmd) == 1)
	{
		g_sig.sig_int = -1;
		g_sig.sig_quit = -1;
		p->env = shlvl(1, p->env);
	}
	elem->next->pid = fork();
	if (elem->next->pid < 0)
		return (-1);
	if (elem->next->pid == 0)
	{
		if (signal(SIGINT, &sighandler) == SIG_ERR || signal(SIGQUIT, &sig_eof) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_in != NULL && opening_in(elem->next->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->next->file_out != NULL && opening_out(elem->next->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		close(elem->fd[1]);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || dup2(elem->fd[0], STDIN_FILENO) < 0)
			error_process(p);
		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(p->paths, elem->next->cmd, p, start) == -1)
			error_process(p);
		exit(0);
	}
	if (wait_all(start) == -1)
		return(-1); //gestion d'erreur avec les kill
	return (0);
}