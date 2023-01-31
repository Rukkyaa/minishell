/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-freebis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 10:25:01 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/31 10:08:49 by gatsby           ###   ########.fr       */
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

int	check_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (is_whitespace(line[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	potential_name(char c)
{
	if (ft_isprint(c) == 1 && c != '<' && c != '>'
		&& c != '|' && c != '&' && is_whitespace(c) == 0
		&& (c < 35 || c > 47))
		return (1);
	return (0);
}

int	end_redir(int alert, char *cmd)
{
	if (alert > -1)
	{
		g_sig.cmd_stat = 2;
		ft_putstr_fd("syntax error near unexpected token `", 2);
		ft_putchar_fd(cmd[alert], 2);
		ft_putstr_fd("\'\n", 2);
		return (-1);
	}
	return (1);
}
