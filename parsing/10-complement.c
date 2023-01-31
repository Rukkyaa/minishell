/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10-complement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:20:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/31 10:00:41 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	countbis(int i, char d, char *line)
{
	while (line[i] != '\0' && line[i] != d)
		i++;
	return (i);
}

int	countofquotes(char *line, char c, int compt)
{
	int	i;

	i = -1;
	if (!(*line))
		return (0);
	while (++i < ft_strlen(line))
	{
		if (line[i] != '\0' && line[i] == '\"' && c == '\'')
			i = countbis(++i, '\"', line);
		else if (line[i] != '\0' && line[i] == '\'' && c == '\"')
			i = countbis(++i, '\'', line);
		else if (line[i] != '\0' && line[i] == c)
		{
			compt++;
			i++;
			while (line[i] != '\0' && line[i] != c)
				i++;
			if (line[i] == c)
				compt++;
		}
	}
	return (compt);
}

int	invalid_quote(char *line)
{
	if (countofquotes(line, '\"', 0) % 2 != 0
		|| countofquotes(line, '\'', 0) % 2 != 0)
		return (ft_putstr_fd("syntax error, unexpected quotes number\n", 2),
			g_sig.cmd_stat = 2, 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '|'
		&& line[ft_strlen(line) - 1] == '|')
		return (ft_putstr_fd("syntax error near unexpected token `||'\n", 2),
			g_sig.cmd_stat = 2, 1);
	else if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '|')
		return (ft_putstr_fd("syntax error near unexpected token `|'\n", 2),
			g_sig.cmd_stat = 2, 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '&'
		&& line[ft_strlen(line) - 1] == '&')
		return (ft_putstr_fd("syntax error near unexpected token `&&'\n", 2),
			g_sig.cmd_stat = 2, 1);
	return (0);
}

int	invalid_start(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i + 1 < ft_strlen(line) && line[i] == '|' && line[i + 1] == '|')
		return (ft_putstr_fd("syntax error near unexpected token `||'\n", 2),
			g_sig.cmd_stat = 2, 1);
	else if (i + 1 < ft_strlen(line) && line[i] == '&' && line[i + 1] == '&')
		return (ft_putstr_fd("syntax error near unexpected token `&&'\n", 2),
			g_sig.cmd_stat = 2, 1);
	else if (line[i] == '|')
		return (ft_putstr_fd("syntax error near unexpected token `|'\n", 2),
			g_sig.cmd_stat = 2, 1);
	return (0);
}

int	increment_amb(char *line, int i)
{
	if (line[i + 1] == '>' || line[i + 1] == '<')
		i += 2;
	else
		i++;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	return (i);
}
