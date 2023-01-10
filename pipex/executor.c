/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/10 15:59:18 by gduhau           ###   ########.fr       */
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

int	exec_command(char **paths, char **cmd, char **env)
{
	int		i;
	char	*path;

	i = -1;
	if (!cmd || ft_strlen(cmd[0]) == 0)
		return (ft_putstr_fd("'' : command not found\n", 2), -1);
	// if (path_comp_builtins(cmd) > 0)
	// 	exec_builtin(path_comp_builtins(cmd), cmd);
	while (paths[++i] != NULL)
	{
		path = ft_strjoin_spe(paths[i], cmd[0]);
		if (path == NULL)
			return (-1);
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) != 0)
				return (perror(""), free(path), -1);
			if (execve(path, cmd, env) == -1)
				return (free(path), -1);
			return (free(path), 0);
		}
		free(path);
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (-1);
}

void error_process(t_all *p)
{
	free_start(p->start, 0);
	free_tab(p->paths);
	free_here_docs(p->here_docs);
	free(p);
	exit(1);
}

int	exec_command_one(t_minishell *elem, t_all *p)
{
	int	status1;

	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1)
			error_process(p);
		if (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1)
			error_process(p);
		if (exec_command(p->paths, elem->cmd, p->env) == -1)
			error_process(p);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) == 1))
		return (-1);
	return (0);
}

int	pipex(t_minishell *first_elem, t_all *p)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, p));
	return (first_pipe(first_elem, p));
}

int	executor(t_tree *start, t_all *p)
{
	int status;

	if (start == NULL || !start)
		return (0);
	status = pipex(start->first_elem, p);
	free(start->cmd);
	if (status == -1)
		free_minishell(start->first_elem, 1);
	else
		free_minishell(start->first_elem, 0);
	if (start->and != NULL && status != -1)
		return (executor(start->and, p));
	else if (start->or != NULL && status == -1)
		return (executor(start->or, p));
	else if (status == -1)
		return (free(start), -1); //quid en cas d'erreur
	return (free(start), 0);
}