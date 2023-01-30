/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-heredocs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 13:15:43 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/30 12:37:13 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*cleanlect(char *lect)
{
	if (lect != NULL)
	{
		free(lect);
		lect = NULL;
	}
	return (lect);
}

void	fin(void)
{
	if (g_sig.sig_int == 1)
		g_sig.cmd_stat = 130;
}

static char	*hdoc_process(t_all *p, int fd, char *newlimiter)
{
	char	*lect;
	int		prev;
	int		nb;

	lect = replace_var(get_next_line(0), p);
	if (lect == NULL && g_sig.sig_int != 1)
		return (warning(newlimiter, 0), NULL);
	prev = prev_valo(lect);
	nb = 1;
	while (ft_strcmp(lect, newlimiter) != 0 && g_sig.sig_int == 0)
	{
		if (lect != NULL)
			write(fd, lect, ft_strlen(lect));
		if (lect != NULL)
			free(lect);
		lect = replace_var(get_next_line(0), p);
		nb++;
		if (prev == 1 && lect == NULL && g_sig.sig_int != 1)
		{
			warning(newlimiter, nb);
			break ;
		}
		prev = prev_valo(lect);
	}
	return (fin(), cleanlect(lect));
}

int	ret_norm(int opt, t_all *p, char *newlimiter, int nb)
{
	if (opt == 0)
	{
		alert_case(newlimiter);
		free(p->here_docs[nb]);
		p->here_docs[nb] = NULL;
	}
	else if (opt == 1)
	{
		free(newlimiter);
		unlink (p->here_docs[nb]);
		free(p->here_docs[nb]);
		p->here_docs[nb] = NULL;
	}
	return (-1);
}

int	fill_file(t_all *p, char **line, int max, int nb)
{
	int		fd;
	char	*newlimiter;
	char	*lect;
	int		alert;

	alert = 0;
	if (nb == max)
		return (p->here_docs[nb] = NULL, 1);
	p->here_docs[nb] = generate_name(ft_trimhard(find_lim(*line, &alert)));
	if (alert == -1 || p->here_docs[nb] == NULL)
		return (alert_case(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	fd = open(p->here_docs[nb], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (free(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	newlimiter = gen_new_limiter(ft_trimhard(find_lim(*line, &alert)));
	if (newlimiter == NULL || alert == -1 || signals_hdoc(0) == -1)
		return (ret_norm(0, p, newlimiter, nb));
	lect = hdoc_process(p, fd, newlimiter);
	if (nb + 1 == max && (lect || !lect))
		lect = get_next_line(-42);
	*line = clean_heredoc_line(*line, p->here_docs[nb],
			find_lim(*line, &alert), &alert);
	if (signals_hdoc(1) == -1 || *line == NULL || g_sig.sig_int == 1)
		return (close (fd), ret_norm(1, p, newlimiter, nb));
	return (free(newlimiter), close (fd), fill_file(p, line, max, ++nb));
}
