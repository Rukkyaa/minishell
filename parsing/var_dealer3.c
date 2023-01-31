/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 10:14:56 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/31 10:22:55 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	var_empt(char *var)
{
	int	i;

	i = 0;
	if (!var || ft_strlen(var) == 0)
		return (0);
	while (i < ft_strlen(var) && var[i] != '\0')
	{
		if (var[i++] != '\v')
			return (0);
	}
	return (1);
}

char	*special_back(char *line, int *i)
{
	line[*i] = '\v';
	*i += 2;
	return (line);
}

char	**lsfact(char **tabl)
{
	int	i;
	int	d;

	i = -1;
	while (tabl[++i] != NULL)
	{
		d = -1;
		while (tabl[i][++d] != '\0')
		{
			if (tabl[i][d] == '\\')
				tabl[i][d] = ' ';
		}
	}
	return (tabl);
}

int	all_rest(char *line, int i)
{
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i == ft_strlen(line))
		return (1);
	return (0);
}

int	pipe_in(char *str)
{
	int	i;

	i = -1;
	while (++i < ft_strlen(str))
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = avoid_quotes(str, i) - 1;
		else if (str[i] == '|')
			return (1);
	}
	return (0);
}
