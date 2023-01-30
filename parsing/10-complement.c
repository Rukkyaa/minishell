/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10-complement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:20:17 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/30 10:48:27 by gduhau           ###   ########.fr       */
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

int	ambiguous(char *line, t_env *env)
{
	int	i;
	int	leng;
	char *var;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i + 1 < ft_strlen(line) && (line[i] == '>' || line[i] == '<'))
	{
		if (line[i + 1] == '>' || line[i + 1] == '<')
			i += 2;
		else
			i++;
		while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
			i++;
		if (i < ft_strlen(line) && line[i] == '$')
		{
			var = get_var(line, env, i, &leng);
			if (var_empt(var) == 1)
			{
				while (i < ft_strlen(line) && is_whitespace(line[i]) == 0)
					printf("%c", line[i++]);
				return (free(line), free(var), 1);
			}
			free(var);
		} 
	}
	return (free(line), 0);
}

int	empty_redir(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	while (i < ft_strlen(line) && (line[i] == '>' || line[i] == '<'))
		i++;
	if (i < ft_strlen(line) && line[i] != ' ' && line[i] != '<' && line[i] != '>')
		return (0);
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i == ft_strlen(line))
		return (1);
	return (0);
}

int	first_check(char *line, t_env *env)
{
	char	*line_bis;
	char	**tab_cmd;

	if (ambiguous(ft_strdup(line), env) == 1)
		return (printf(": ambiguous redirect\n"), 1);
	if (ft_strlen(line) == 1 && line[0] == '.')
		return (printf(".: filename argument required\n"), 1);
	if (empty_redir(line) == 1)
		return (printf("syntax error near unexpected token `newline'\n"), 1);
	line_bis = erase_redir(ft_strdup(line));
	if (check_spaces(line_bis, ' ') == 1 || que_space(line_bis) == 1)
		return (free(line_bis), 0);
	tab_cmd = ft_split_spe(line_bis, '|');
	if (pipe_in(line_bis) == 1 && all_spaces(tab_cmd) == 1)
		return (printf("syntax error near unexpected token `|'\n"),
			free_tab(tab_cmd), free(line_bis), g_sig.cmd_stat = 2, 1);
	return (free_tab(tab_cmd), free(line_bis), 0);
}
