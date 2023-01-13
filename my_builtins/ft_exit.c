/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:23:26 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/13 16:00:47 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_exit(t_all *p)
{
	free(g_sig.line);
	free_start(p->start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	exit(134);
}