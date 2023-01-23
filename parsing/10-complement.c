/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10-complement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:20:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/23 14:27:49 by gduhau           ###   ########.fr       */
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
	int i;

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

int	invalid_quote(char *line)
{
	if (countofquotes(line, '\"', 0) % 2 != 0
		|| countofquotes(line, '\'', 0) % 2 != 0)
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '|'
		&& line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '&'
		&& line[ft_strlen(line) - 1] == '&')
		return (printf("Syntax error\n"), 1);
	return (0);
}

int	first_check(char *line)
{
	char *line_bis;
	char **tab_cmd;

	line_bis = erase_redirbis(ft_strdup(line));
	tab_cmd = ft_split_spe(line_bis, '|');
	if (all_spaces(tab_cmd, line_bis) == 1)
		return (printf("syntax error near unexpected token `|'\n"),
			free_tab(tab_cmd), free(line_bis), g_sig.cmd_stat = 2, 1);
	return (free_tab(tab_cmd), free(line_bis), 0);
}