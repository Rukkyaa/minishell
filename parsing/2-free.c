/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-free.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 00:02:09 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 11:06:12 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_files_in(t_infile *lst)
{
	t_infile	*p;

	if (lst == NULL || !lst)
		return ;
	while (lst != NULL)
	{
		if (lst->file_in && lst->file_in != NULL)
			free(lst->file_in);
		p = lst;
		lst = lst->next;
		free(p);
	}
	return ;
}

void	free_env(t_env *envp)
{
	t_env	*temp;

	if (!envp || envp == NULL)
		return ;
	while (envp != NULL)
	{
		if (!(!envp->key || envp->key == NULL))
			free(envp->key);
		if (!(!envp->value || envp->value == NULL))
			free(envp->value);
		temp = envp;
		envp = envp->next;
		free(temp);
	}
	return ;
}

void	free_files_out(t_outfile *lst, int mode)
{
	t_outfile	*p;

	if (lst == NULL || !lst)
		return ;
	while (lst != NULL)
	{
		if (mode == 1 && lst->created == 1 && lst->file_out
			&& lst->file_out != NULL)
			unlink(lst->file_out);
		if (lst->file_out && lst->file_out != NULL)
			free(lst->file_out);
		p = lst;
		lst = lst->next;
		free(p);
	}
	return ;
}

void	free_minishell(t_minishell *elem, int mode)
{
	if (!elem || elem == NULL)
		return ;
	if (elem->file_in != NULL)
		free_files_in(elem->file_in);
	if (elem->file_out != NULL)
		free_files_out(elem->file_out, mode);
	free_tab(elem->cmd);
	if (elem->next && elem->next != NULL)
		free_minishell(elem->next, mode);
	free(elem);
}

void	free_start(t_tree *start, int mode)
{
	if (start == NULL || !start)
		return ;
	free(start->cmd);
	free_minishell(start->first_elem, mode);
	if (start->and != NULL)
		free_start(start->and, mode);
	if (start->or != NULL)
		free_start(start->or, mode);
	free(start);
	start = NULL;
}
