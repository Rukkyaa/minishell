/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/05 11:34:02 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	opening(char *file, int port, int append, int mode)
{
	int fdt;
	char *line;

	if (file == NULL || !file)
		return (-1);
	if (mode == 0 && (access(file, F_OK) != 0 || access(file, R_OK) != 0))
		return (perror(""), -1);
	else if (mode == 1 && (access(file, F_OK) == 0 && access(file, W_OK) != 0))
		return (perror(""), -1);
	else if (mode == 1 && access(file, F_OK) != 0)
		fdt = open(file, O_WRONLY | O_CREAT, S_IRWXU);
	else if (mode == 1)
		fdt = open(file, O_WRONLY);
	else if (mode == 0)
		fdt = open(file, O_RDONLY);
	else
		fdt = -1;
	if (append == 1 && mode == 1 && fdt != -1)
	{
		line = get_next_line(fdt);
		while (line != NULL)
		{
			free(line);
			line = get_next_line(fdt);
		}
	}
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

	// printf("Debut de exec_command_one\n");
	// printf("Fin de exec_command_one\n");
	// printf("%p%p", paths, env);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->infile != NULL && opening(elem->infile, STDIN_FILENO, 0, 0) == -1)
			exit (42);
		if (elem->outfile != NULL && opening(elem->outfile, STDOUT_FILENO, elem->append, 1) == -1)
			exit (42);
		if (exec_command(paths, elem->cmd, env) == -1) //|| closing(f_in, f_out) == -1)
			exit(42);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || (WIFEXITED(status1) && WEXITSTATUS(status1) == 42))
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
	if (start->and != NULL && status != -1)
		return (executor(start->and));
	else if (start->or != NULL && status == -1)
		return (executor(start->or));
	else if (status == -1)
		return (-1); //quid en cas d'erreur
	return (0);
}