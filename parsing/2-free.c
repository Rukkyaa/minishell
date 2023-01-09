/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-free.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 00:02:09 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/09 18:43:48 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_files_in(t_infile *lst)
{
	if (lst == NULL || !lst)
		return ;
	while (lst->next != NULL)
	{
		free(lst->file_in);
		lst = lst->next;
	}
	free(lst->file_in);
	return ;
}

void free_files_out(t_outfile *lst, int mode)
{
	if (lst == NULL || !lst)
		return ;
	while (lst->next != NULL)
	{
		if (mode == 1 && lst->created == 1)
			unlink(lst->file_out);
		free(lst->file_out);
		lst = lst->next;
	}
	free(lst->file_out);
	return ;
}

void	free_minishell(t_minishell *elem, int mode)
{
	if (!elem || elem == NULL)
		return ;
	if (elem->file_in != NULL)
		free_files_in(elem->file_in);
	if (elem->file_out != NULL)
		free_files_out(elem->file_out, 1);
	free_tab(elem->cmd);
	free_minishell(elem->next, mode);
	free(elem);
}

void	free_start(t_tree *start)
{
	free(start->cmd);
	free_minishell(start->first_elem, 1);
	if (start->and != NULL)
		free_start(start->and);
	if (start->or != NULL)
		free_start(start->or);
	free(start);
}

void	free_here_docs(char **here_docs)
{
	int	i;

	i = 0;
	if (here_docs == NULL)
		return ;
	while (here_docs[i] != NULL)
	{
		unlink(here_docs[i]); //check si il y a pas une secu a mettre
		free(here_docs[i]);
		i++;
	}
	free(here_docs);
}

// void free_cmd(t_all *p)
// {
// 	//free_tab(p->paths);
// 	free_start(p->start);
// 	free_here_docs(p->here_docs);
// }

void free_all(t_all *p)
{
	free_tab(p->paths);
	free(p);
}