/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-operations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:47:45 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/25 11:43:05 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	op_found(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0' && line[i + 1] != '\0')
	{
		if (line[i] != '\0' && (line[i] == '\"' || line[i] == '\''))
		{
			i = avoid_quotes(line, i);
			if (i == ft_strlen(line))
				break ;
		}
		if (line[i] == '&' && line[i + 1] == '&')
			return (i);
		if (line[i] == '|' && line[i + 1] == '|')
			return (i);
		i++;
	}
	return (-1);
}

char	*clean_rest_op(t_tree *start, int index)
{
	int	i;

	i = index;
	while (start->cmd[i] != '\0')
		start->cmd[i++] = ' ';
	return (start->cmd);
}

char	*clean_res(char *reserve, int index)
{
	int	i;

	i = 0;
	while (i < index)
		reserve[i++] = ' ';
	return (reserve);
}

// void	clean_res(char *reserve, int index)
// {
// 	int	i;

// 	i = 0;
// 	while (i < index)
// 		reserve[i++] = ' ';
// }

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (0);
	else if ((!s1 && s2) || (s1 && !s2))
		return (-1);
	while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	avoid_quotes_spe(char *line, int i)
{
	int	ibis;

	ibis = i;
	if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\0' && line[i] != '\'')
			i++;
	}
	if (line[i] == '\0')
		return (++ibis);
	return (++i);
}