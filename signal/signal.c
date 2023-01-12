/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:22:03 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/12 17:37:53 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_signal(t_all *p)
{
    g_sig.sig_quit = 0;
    g_sig.sig_int = 0;
	g_sig.p = p;
}

void    sig_quit(int code)
{
	(void) code;
    printf("Sig quit\n");
    g_sig.sig_quit = 1;
}

void    sig_int(int code)
{
    (void)code;
	//ajouter les kill
	free_start(g_sig.p->start, 0);
	free_here_docs(g_sig.p->here_docs);
	//if (g_sig.p->line != NULL) //segfautl
		//free(g_sig.p->line);
	free_all(g_sig.p);
    printf("Sig int\n");
    g_sig.sig_int = 1;
	exit(0);
}
