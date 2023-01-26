/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-freebis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 10:25:01 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/26 11:06:55 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_here_docs(char **here_docs)
{
	int	i;

	i = 0;
	if (here_docs == NULL)
		return ;
	while (here_docs[i] != NULL)
	{
		unlink(here_docs[i]);
		free(here_docs[i]);
		i++;
	}
	free(here_docs);
}

void	free_all(t_all *p)
{
	free_env(p->env);
	free(p);
}
