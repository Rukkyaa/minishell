/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-initbis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:03:16 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/24 01:09:07 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_interligne(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '(')
		{
			while (line[i] != '\0' && line[i + 1] != '\0' && ((line[i] == '&'
				&& line[i + 1] == '&') || (line[i] == '|' && line[i + 1] == '|')))
			{
				if (line[i++] == ')')
					return (0);
			}
			while (line[i] != '\0' && line[i] != ')')
				i++;
			if (line[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_extraligne(char *line)
{
	int	i;
	int	count;

	i = 0;
	while (line[i] != '\0')
	{
		count = 0;
		if (line[i] == ')')
		{
			while (line[i] != '\0' && line[i + 1] != '\0' && line[i + 1] != '(')
			{
				if (line[i] != '\0' && line[i + 1] != '\0' && ((line[i] == '&' && line[i + 1] == '&') || (line[i] == '|' && line[i + 1] == '|')))
					count++;
				i++;
			}
			if ((line[i] != '\0' && line[i + 1] != '\0') && count == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

char *clean_first(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0' && is_whitespace(line[i]) == 1)
		i++;
	if (line[i] != '\0' && line[i] == '(')
	{
		line[i] = ' ';
		while (line[i] != '\0' && line[i] != ')')
			i++;
		if (line[i] != '\0' && line[i] == ')')
			line[i] = ' ';
	}
	return (line);
}