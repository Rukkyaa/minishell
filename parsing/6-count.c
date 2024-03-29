/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6-count.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 17:29:25 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/30 10:54:57 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	countof_spe(char **line, char c, int compt, int i)
{
	if (!(*line))
		return (0);
	while (i < ft_strlen(*line) && (*line)[++i] != '\0')
	{
		if (i < ft_strlen(*line) && (*line)[i] != '\0' && (*line)[i] == '\"')
		{
			i++;
			while ((*line)[i] != '\0' && (*line)[i] != '\"')
				i++;
			i++;
		}
		if (i < ft_strlen(*line) && (*line)[i] != '\0' && (*line)[i] == '\'')
		{
			i++;
			while (i < ft_strlen(*line) && (*line)[i] != '\'')
				i++;
			i++;
		}
		if (i < ft_strlen(*line) && (*line)[i] == c)
			compt++;
		if (i >= ft_strlen(*line))
			break ;
	}
	return (compt);
}

int	avoid_quotes(char *line, int i)
{
	int	ibis;

	ibis = i;
	if (line[i] == '\"')
	{
		i++;
		while (line[i] != '\0' && line[i] != '\"')
			i++;
	}
	else if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\0' && line[i] != '\'')
			i++;
	}
	if (line[i] == '\0')
		return (++ibis);
	return (++i);
}

int	last_char_spe(char *line, char c)
{
	int	i;
	int	res;

	i = 0;
	res = -1;
	while (line[i] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		if (i == ft_strlen(line))
			break ;
		if (line[i] == c)
			res = i;
		i++;
	}
	return (res);
}

int	first_char_spe(char *line, char c, int index)
{
	int	i;

	i = index;
	while (i < ft_strlen(line) && line[i] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		if (line[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	count_pipe(char *cmd)
{
	int	i;
	int	compt;

	i = 0;
	compt = 1;
	if (!cmd || cmd == NULL)
		return (0);
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
		{
			i = avoid_quotes(cmd, i);
			if (i == ft_strlen(cmd))
				break ;
		}
		if (cmd[i] == '|')
			compt++;
		i++;
	}
	return (compt);
}
