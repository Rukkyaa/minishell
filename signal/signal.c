/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:22:03 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/24 01:21:25 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	event(void) 
{
	return (42);
}

int	create_signal(void)
{
	struct termios		old_termios;
	struct termios		new_termios;
	struct sigaction	a;

	if (tcgetattr(0,&old_termios) != 0)
		return (-1);
	new_termios = old_termios;
	new_termios.c_cc[VSUSP]  = 4;
	//new_termios.c_iflag = IXOFF;
	if (tcsetattr(0,TCSANOW,&new_termios))
		return (-1);
	a.sa_handler = sighandler;
	a.sa_flags = 0;
	sigemptyset( &a.sa_mask );
	signal(SIGQUIT, SIG_IGN);
	if (sigaction( SIGINT, &a, NULL) != 0 || sigaction(SIGTSTP, &a, NULL) != 0)
		//|| sigaction(SIGQUIT, &a, NULL) != 0)
		return (-1);
	return (0);
}

int	create_signal_spe(void)
{
	struct termios		old_termios;
	struct termios		new_termios;
	struct sigaction	a;

	if (tcgetattr(0,&old_termios) != 0)
		return (-1);
	new_termios = old_termios;
	new_termios.c_cc[VEOF]  = 4;
	new_termios.c_cc[VSUSP]  = 26;
	if (tcsetattr(0,TCSANOW,&new_termios))
		return (-1);
	a.sa_handler = sighandler;
	a.sa_flags = 0;
	sigemptyset( &a.sa_mask );
	if (sigaction( SIGINT, &a, NULL) != 0
		|| sigaction(SIGQUIT, &a, NULL) != 0)
		return (-1);
	return (init_signal(-1), 0);
}

int	create_signal_here(void)
{
	struct termios		old_termios;
	struct termios		new_termios;
	struct sigaction	a;

	if (tcgetattr(0,&old_termios) != 0)
		return (-1);
	new_termios = old_termios;
	new_termios.c_cc[VEOF]  = 4;
	new_termios.c_cc[VSUSP]  = 26;
	if (tcsetattr(0,TCSANOW,&new_termios))
		return (-1);
	a.sa_handler = sighandler;
	a.sa_flags = 0;
	sigemptyset(&a.sa_mask );
	if (sigaction(SIGINT, &a, NULL) != 0)
		return (-1);
	return (init_signal(0), 0);
}

void	init_signal(int nb)
{
	g_sig.sig_quit = nb;
	g_sig.sig_int = nb;
	g_sig.p_status = nb;
}

void	sig_eof(int code)
{
	//int n;

	(void) code;
	if (g_sig.p_status == 0 && rl_end == 0)
	{
		g_sig.sig_quit = 1;
		rl_on_new_line(); // Regenerate the prompt on a newline
		rl_replace_line("end", 0);
		rl_done = 1;
	}
	else if (g_sig.p_status == 2)
	{
		// if (ioctl(0, FIONREAD, &n) == -1)
		// 	printf("error\n");
		// printf("%d\n", n);
		// if (n > 0)
		// 	printf("gagne\n");
		// else
		// 	printf("perdu");
		g_sig.sig_quit = 1;
		//write(0, "\0", 1);
	}
}

void	sighandler(int code)
{
	if (code == (int)SIGINT)
	{
		g_sig.sig_int = 1;
		if (g_sig.p_status == 0)
		{
			g_sig.cmd_stat = 130;
			rl_done = 1;
		}
	}
	else if (code == (int)SIGTSTP)
		sig_eof(code);
	else if (code == (int)SIGQUIT && g_sig.p_status == -1)
		return ;
	return ;
}
