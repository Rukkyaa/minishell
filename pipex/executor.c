/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/25 17:45:18 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **env_to_char(t_env *env)
{
	char **reforged;
	int i;
	int e;
	t_env *temp;

	i = 0;
	e = 0;
	if (!env || env == NULL)
		return (NULL);
	temp = env;
	while (temp != NULL && i++ > -1)
		temp = temp->next;
	reforged = malloc((i + 1) * sizeof(char *));
	if (!reforged)
		return (NULL);
	while (e < i)
	{
		if (env->code == 1 || env->code == 2 || env->code == 4)
		{
			reforged[e] = ft_strjoin(env->key, "=");
			reforged[e] = ft_strjoin_spe(reforged[e], env->value);
			if (reforged[e++] == NULL)
				return (free_tab(reforged), NULL);
		}
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

int	stop_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR
		|| signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		return (1);
	return (0);
}

int	cond_redir(t_minishell *elem, t_all *p)
{
	if (g_sig.sig_int > 0 || g_sig.sig_quit > 0
		|| (elem->file_in != NULL
			&& opening_in(elem->file_in, STDIN_FILENO, elem->cmd, p) == -1)
		|| (elem->file_out != NULL
			&& opening_out(elem->file_out, STDOUT_FILENO, elem->cmd, p) == -1))
		return (-1);
	return (0);
}


int	exec_command_one(t_minishell *elem, t_all *p, t_tree *start)
{
	int	status1;

	// if (elem->cmd == NULL || elem->cmd[0] == NULL)
	// 	return (0);
	if (path_comp_builtins(elem->cmd) < 0)
		return (exec_builtin(path_comp_builtins(elem->cmd), elem->cmd, p, start));
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
		{
			//close_ports(elem);
			end_process(p, 1);
		}
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) == -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (create_signal() , init_signal(0), WEXITSTATUS(status1));
	return (create_signal() , init_signal(0), 0);
}

// int	exec_command_one(t_minishell *elem, t_all *p, t_tree *start)
// {
// 	int	status1;

// 	if (path_comp_builtins(elem->cmd) < 0)
// 		return (exec_builtin(path_comp_builtins(elem->cmd), elem->cmd, p, start));
// 	if (stop_signals() == 1)
// 		return (134);
// 	elem->pid = fork();
// 	if (elem->pid < 0)
// 		return (-1);
// 	if (elem->pid == 0)
// 	{
// 		if (create_signal_spe() == -1)
// 			end_process(p, 1);
// 		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
// 			end_process(p, 1);
// 		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
// 			end_process(p, 1);
// 		if (g_sig.sig_int > 0 || g_sig.sig_quit > 0 || exec_command(p->paths, elem->cmd, p, start) != 0)
// 			end_process(p, 1);
// 		exit(0);
// 	}
// 	if (waitpid(elem->pid, &status1, 0) == -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
// 		return (create_signal() , init_signal(0), WEXITSTATUS(status1));
// 	return (create_signal() , init_signal(0), 0);
// }

int	pipex(t_minishell *first_elem, t_all *p, t_tree *start)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, p, start));
	else if (first_elem->next != NULL && first_elem->next->next != NULL
		&& find_cat(first_elem) == 1)
		return (first_pipe_cat(first_elem, p, start));
	return (first_pipe(first_elem, p, start));
}

//void kill_process(t_tree *start, t_all *p, char *line)
//{
//	free_start(start, 1);
//	free_here_docs(p->here_docs);
//	free(line);
//	free_all(p);
//	clear_history();
//	exit (0);
//}

int	executor(t_tree *start, t_all *p, char *line)
{
	t_tree	*intermed;

	if (start == NULL || !start)
		return (-1);
	g_sig.cmd_stat = pipex(start->first_elem, p, start);
	init_signal(0);
	if (g_sig.cmd_stat != 0)
	{
		if (start->or && start->or != NULL)
		{
			free(start->cmd);
			free_minishell(start->first_elem, 0);
			return (intermed = start->or, free(start),
				p->start = intermed, executor(p->start, p, line));
		}
		return (free_start(start, 1), g_sig.cmd_stat);
	}
	if (start->and && start->and != NULL)
	{
		free(start->cmd);
		free_minishell(start->first_elem, 0);
		return (intermed = start->and, free(start),
			p->start = intermed, executor(p->start, p, line));
	}
	return(free_start(start, 0), g_sig.cmd_stat);
}