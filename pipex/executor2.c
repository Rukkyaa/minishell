/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/22 17:46:09 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	opening_out(t_outfile *file_org, int port)
{
	int fdt;
	t_outfile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (file->file_out == NULL)
			return (printf(" : No such file or directory\n"), -1);
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
	if (file->file_out == NULL)
		return (printf(" : No such file or directory\n"), -1);
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
		if (file->file_in == NULL)
			return (printf(" : No such file or directory\n"), -1);
		if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
			return (perror(file->file_in),  -1);
		file = file->next;
	}
	if (file->file_in == NULL)
		return (printf(" : No such file or directory\n"), -1);
	if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
		return (perror(file->file_in), -1);
	fdt = open(file->file_in, O_RDONLY);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

int	find_cat(t_minishell *elem)
{
	t_minishell	*p;

	p = elem;
	while (p != NULL)
	{
		if (ft_strcmp(p->cmd[0], "cat") == 0 && p->cmd[1] == NULL
			&& p->file_in == NULL && p->file_out == NULL)
			return (1);
		p = p->next;
	}
	return (0);
}