/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/09 18:40:51 by gduhau           ###   ########.fr       */
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
	char *line;
	t_outfile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
			return (perror(""), -1);
		else if (access(file->file_out, F_OK) != 0)
		{
			fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
			if (fdt == -1)
				return (perror(""), -1);
			if (close(fdt) == -1)
				return (perror(""), -1);
		}
		file = file->next;
	}
	if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
		return (perror(""), -1);
	else if (access(file->file_out, F_OK) != 0)
		fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
	else
		fdt = open(file->file_out, O_WRONLY);
	if (file->append == 1 && fdt != -1)
	{
		line = get_next_line(fdt);
		while (line != NULL)
		{
			free(line);
			line = get_next_line(fdt); //Besoin du get next line 42 ??
		}
	}
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
			return (perror(""), -1);
		file = file->next;
	}
	if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
		return (perror(""), -1);
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
	if (path_comp_builtins(cmd) > 0)
		exec_builtin(path_comp_builtins(cmd), cmd);
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

int	exec_command_one(t_minishell *elem, char **paths, char **env)
{
	int	status1;

	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1)
			exit (1);
		if (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1)
			exit (1);
		if (exec_command(paths, elem->cmd, env) == -1)
			exit(1);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) == 1))
		return (-1);
	return (0);
}

int	pipex(t_minishell *first_elem)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, first_elem->paths, first_elem->env));
	return (first_pipe(first_elem, first_elem->paths, first_elem->env));
}

int	executor(t_tree *start)
{
	int status;

	if (start == NULL || !start)
		return (0);
	status = pipex(start->first_elem);
	free(start->cmd);
	if (status == -1)
		free_minishell(start->first_elem, 1);
	else
		free_minishell(start->first_elem, 0);
	if (start->and != NULL && status != -1)
		return (executor(start->and));
	else if (start->or != NULL && status == -1)
		return (executor(start->or));
	else if (status == -1)
		return (free(start), -1); //quid en cas d'erreur
	return (free(start), 0);
}