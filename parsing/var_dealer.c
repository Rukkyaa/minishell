/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:04:39 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/07 18:05:57 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int avoid_quotes_spe(char *line, int i)
{
	int ibis;

	ibis = i;
	if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\0' && line[i] != '\'')
			i++;
	}
	if (line[i] == '\0')
		return(++ibis);
	return (++i);
}

static char *get_var(char **line, char **env, int i)
{
	int e;
	char *var;
	int len;
	int d;

	e = i + 1;
	while ((*line)[e] != '\0' && is_whitespace((*line)[e]) == 0)
		e++;
	var = ft_substr(*line, i + 1, e - i - 1);
	if (!var)
		return (NULL);
	d = e;
	e = -1;
	len = ft_strlen(var);
	while (env[++e] != NULL)
	{
		if (ft_strncmp(env[e], var, len) == 0)
		{
			while (i < d)
				(*line)[i++] = ' ';
			return (free(var), ft_substr(env[e], len + 1, ft_strlen(env[e]) - len - 1));
		}
	}
	e = 0;
	while (var[e] != '\0')
		var[e++] = ' ';
	return (var);
}

static char *change_line(char *line, char *var, int i)
{
	char *new_line;
	char *reste;

	printf("%s\n", var);
	if (!var || var == NULL)
		return (free(line), NULL);
	new_line = ft_substr(line, 0, i + 1);
	if (new_line == NULL || !new_line)
		return (free(line), free(var), NULL);
	new_line = ft_strjoin_spe(new_line, var);
	if (new_line == NULL || !new_line)
		return (free(line), free(var), NULL);
	reste = ft_substr(line, i + ft_strlen(var) + 1, ft_strlen(line));
	if (reste == NULL || !reste)
		return (free(line), free(var), free(new_line), NULL);
	new_line = ft_strjoin_spe(new_line, reste);
	if (new_line == NULL || !new_line)
		return (free(reste), free(var), free(line), free(new_line), NULL);
	return (free(reste), free(var), new_line);
}

char	**replace_var(char **line, char **env)
{
	int	i;

	i = -1;
	if (*line == NULL || line == NULL)
		return (NULL);
	while ((*line)[++i] != '\0')
	{
		if ((*line)[i] == '\'')
			i = avoid_quotes_spe(*line, i);
		if ((*line)[i] == '$' && (*line)[i + 1] != '\0' &&  is_whitespace((*line)[i + 1]) == 0)
		{
			//if ((*line)[i + 1] != '\0' && (*line)[i + 1] == '?')
				//WHAT THE FUCK
			(*line)[i] = ' ';
			*line = change_line(*line, get_var(line, env, i), i);
			if (*line == NULL)
				return (NULL);
		}
	}
	return (line);
}