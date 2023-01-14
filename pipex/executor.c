/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/14 23:43:18 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	opening(char *file, int port, int append, int mode)
// {
// 	int fdt;
// 	char *line;

// 	if (file == NULL || !file)
// 		return (-1);
// 	if (mode == 0 && (access(file, F_OK) != 0 || access(file, R_OK) != 0))
// 		return (perror(""), -1);
// 	else if (mode == 1 && (access(file, F_OK) == 0 && access(file, W_OK) != 0))
// 		return (perror(""), -1);
// 	else if (mode == 1 && access(file, F_OK) != 0)
// 		fdt = open(file, O_WRONLY | O_CREAT, S_IRWXU);
// 	else if (mode == 1)
// 		fdt = open(file, O_WRONLY);
// 	else if (mode == 0)
// 		fdt = open(file, O_RDONLY);
// 	else
// 		fdt = -1;
// 	if (append == 1 && mode == 1 && fdt != -1)
// 	{
// 		line = get_next_line(fdt);
// 		while (line != NULL)
// 		{
// 			free(line);
// 			line = get_next_line(fdt);
// 		}
// 	}
// 	if (fdt == -1)
// 		return (perror(""), -1);
// 	if (dup2(fdt, port) < 0)
// 		return (perror(""), close(fdt), -1);
// 	return (close(fdt), 0);
// }

int	opening_out(t_outfile *file_org, int port)
{
	int fdt;
	t_outfile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
			return (perror(file->file_out), -1);
		else
		{
			if (file->append == 1)
				fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			else
				fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
			if (fdt == -1)
				return (perror(""), -1);
			if (close(fdt) == -1)
				return (perror(""), -1);
		}
		file = file->next;
	}
	if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
		return (perror(file->file_out), -1);
	else if (file->append == 1)
		fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
	else
		fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

int	opening_in(t_infile *file_org, int port)
{
	int fdt;
	t_infile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while(file->next != NULL)
	{
		if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
			return (perror(file->file_in), -1);
		file = file->next;
	}
	if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
		return (perror(file->file_in), -1);
	fdt = open(file->file_in, O_RDONLY);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

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
		if (env->code == 1 || env->code == 2)
		{
			reforged[e] = ft_strjoin(env->key, "=");
			reforged[e] = ft_strjoin_spe(reforged[e], env->value); //securite direc dans le spe pour le reforged
			if (reforged[e++] == NULL)
				return (free_tab(reforged), NULL);
		}
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

int	exec_command(char **paths, char **cmd, t_all *p, t_tree *start)
{
	int		i;
	char	*path;
	char	**reforged_env;

	i = -1;
	if (!cmd || ft_strlen(cmd[0]) == 0)
		return (ft_putstr_fd("'' : command not found\n", 2), -1);
	if (path_comp_builtins(cmd) > 0)
	{
		printf("---entering builtins---\n");
		if (exec_builtin(path_comp_builtins(cmd), cmd, p, start) == EXIT_FAILURE)
			return (-1);
		return (0);
	}
	reforged_env = env_to_char(p->env);
	if (reforged_env == NULL && p->env != NULL)
		return (-1);
	while (paths[++i] != NULL)
	{
		path = ft_strjoin(paths[i], cmd[0]);
		if (path == NULL)
			return (-1); //gesrtion d'erreurs 
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) != 0)
				return (perror(""), free(path), -1);
			if (execve(path, cmd, reforged_env) == -1)
				return (free(path), free_tab(reforged_env), -1);
			return (free(path), free_tab(reforged_env), 0);
		}
		free(path);
	}
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) != 0)
			return (perror(""), free(path), free_tab(reforged_env), -1);
		if (execve(cmd[0], cmd, reforged_env) == -1)
			return (free_tab(reforged_env), -1);
		return (free_tab(reforged_env), 0);
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (free_tab(reforged_env), g_sig.cmd_stat = 127, 0);
}

void error_process(t_all *p)
{
	free_start(p->start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	if (g_sig.sig_quit == 1)
		exit(134);
	else if (g_sig.sig_int == 1)
		exit(0);
	g_sig.cmd_stat = 1;
	exit(1);
}

int	exec_command_one(t_minishell *elem, t_all *p, t_tree *start)
{
	int	status1;

	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (signal(SIGINT, &sighandler) == SIG_ERR || signal(SIGQUIT, &sig_eof) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || exec_command(p->paths, elem->cmd, p, start) == -1)
			error_process(p);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (WEXITSTATUS(status1));
	return (0);
}

int	pipex(t_minishell *first_elem, t_all *p, t_tree *start)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, p, start));
	return (first_pipe(first_elem, p, start));
}

void kill_process(t_tree *start, t_all *p, char *line)
{
	free_start(start, 1);
	free_here_docs(p->here_docs);
	free(line); //A ACTIVER AVEC LE MAIN PRINCIPAL
	//printf("%s\n", line);
	free_all(p);
	exit (0);
}
int	executor(t_tree *start, t_all *p, char *line)
{
	if (start == NULL || !start)
		return (-1);
	p->last_status = pipex(start->first_elem, p, start);
	if (p->last_status == 134)
		kill_process(start, p, line);
	free(start->cmd);
	if (p->last_status != 0) //verifier que c'est bien ce qui est attendu
		free_minishell(start->first_elem, 1);
	else
		free_minishell(start->first_elem, 0);
	if (start->and != NULL && p->last_status == 0)
		return (executor(start->and, p, line));
	else if (start->or != NULL && p->last_status != 0)
		return (executor(start->or, p, line));
	else if (p->last_status != 0)
		return (free(start), p->last_status); //quid en cas d'erreur
	return (free(start), 0);
}