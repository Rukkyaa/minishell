/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-free.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 00:02:09 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 01:04:32 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_minishell(t_minishell *elem)
{
	if (!elem || elem == NULL)
		return ;
	if (elem->infile != NULL)
		free(elem->infile);
	if (elem->outfile != NULL)
		free(elem->outfile);
	free_tab(elem->cmd);
	free_minishell(elem->next);
	free(elem);
}

void	free_start(t_tree *start)
{
	free(start->cmd);
	free_minishell(start->first_elem);
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

void free_cmd(t_all *p)
{
	//free_tab(p->paths);
	free_start(p->start);
	free_here_docs(p->here_docs);
}

void free_all(t_all *p)
{
	free_tab(p->paths);
	free(p);
}