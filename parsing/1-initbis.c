/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-initbis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:03:16 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/30 23:27:28 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_interligne(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && line[i] != '\0')
	{
		if (line[i] == '(')
		{
			while (line[i] != '\0' && line[i + 1] != '\0' && ((line[i] == '&'
						&& line[i + 1] == '&') || (line[i] == '|')))
			{
				if (line[i++] == ')')
					return (0);
			}
			while (i < ft_strlen(line) && line[i] != ')')
				i++;
			if (line[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_extraligne(char *li)
{
	int	i;
	int	count;

	i = 0;
	while (i < ft_strlen(li) && li[i] != '\0')
	{
		count = 0;
		if (li[i] == ')')
		{
			while (li[i] != '\0' && li[i + 1] != '\0' && li[i + 1] != '(')
			{
				if (li[i] != '\0' && li[i + 1] != '\0'
					&& ((li[i] == '&' && li[i + 1] == '&') || (li[i] == '|')))
					count++;
				i++;
			}
			if ((li[i] != '\0' && li[i + 1] != '\0') && count == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

char	*clean_first(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (line[i] != '\0' && line[i] == '(')
	{
		line[i] = ' ';
		while (i < ft_strlen(line) && line[i] != ')')
			i++;
		if (line[i] != '\0' && line[i] == ')')
			line[i] = ' ';
	}
	return (line);
}
