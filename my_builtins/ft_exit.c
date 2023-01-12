/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:23:26 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/12 17:57:57 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_exit(t_all *p)
{
	p->exit = 1;
	// printf("A");
	// free_start(p->start, 0);
	// printf("B");
	// free_tab(p->paths);
	// printf("c");
	// free_env(p->env);
	// printf("D");
	// free_here_docs(p->here_docs);
	// printf("E");
	// free(p);
	printf("F");
	exit(134);
	printf("fuck");
	return (0);
}