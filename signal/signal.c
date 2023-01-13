/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:04:13 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/12 15:18:00 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_signal(void)
{
	g_sig.sig_quit = 0;
	g_sig.sig_int = 0;
}

void	sig_quit(int code)
{
	(void)code;
	printf("Sig quit\n");
	g_sig.sig_quit = 1;
}

void	sig_int(int code)
{
	(void)code;
	printf("Sig int\n");
	g_sig.sig_int = 1;
}