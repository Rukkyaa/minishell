/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:23:26 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/14 23:28:01 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_all *p, t_tree *start)
{
	printf("exit\n");
	free(g_sig.line);
	free_start(start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	exit(134);
}
