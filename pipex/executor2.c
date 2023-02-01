/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/02/01 10:13:42 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_file_in(char *file)
{
	if (file == NULL)
		return (ft_putstr_fd(" : No such file or directory\n", 2), -1);
	if (access(file, F_OK) != 0 || access(file, R_OK) != 0)
		return (perror(file), -1);
	return (0);
}

static int	check_file_out(t_outfile *file)
{
	int	fdt;

	if (file->file_out == NULL)
		return (ft_putstr_fd(" : No such file or directory\n", 2), -1);
	if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
		return (perror(file->file_out), -1);
	else
	{
		if (file->append == 1)
			fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
		else
			fdt = open(file->file_out, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fdt == -1)
			return (perror(""), -1);
		if (close(fdt) == -1)
			return (perror(""), -1);
	}
	return (fdt);
}

int	fin_opening(t_all *p, int fdt, int port, char **cmd)
{
	if (fdt == -1)
		return (perror(""), -1);
	if (cmd == NULL || cmd[0] == NULL)
	{
		close(fdt);
		end_process(p, 0);
	}
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

int	opening_out(t_outfile *file_org, int port, char **cmd, t_all *p)
{
	int			fdt;
	t_outfile	*file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (check_file_out(file) == -1)
			return (-1);
		file = file->next;
	}
	if (file->file_out == NULL)
		return (ft_putstr_fd(" : No such file or directory\n", 2), -1);
	if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
		return (perror(file->file_out), -1);
	else if (file->append == 1)
		fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
	else
		fdt = open(file->file_out, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	return (fin_opening(p, fdt, port, cmd));
}

int	opening_in(t_infile *file_org, int port, char **cmd, t_all *p)
{
	int			fdt;
	t_infile	*file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (check_file_in(file->file_in) == -1)
			return (-1);
		file = file->next;
	}
	if (check_file_in(file->file_in) == -1)
		return (-1);
	fdt = open(file->file_in, O_RDONLY);
	if (fdt == -1)
		return (perror(""), -1);
	if (cmd == NULL || cmd[0] == NULL)
	{
		close(fdt);
		end_process(p, 0);
	}
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}
