/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 21:59:09 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/02/01 14:04:33 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_spaces(char *s, char c)
{
	int	i;
	int	compt;

	i = 0;
	compt = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '\"')
			compt++;
		if (s[i] != '\'' && s[i] != '\"' && s[i] != c)
			return (0);
		i++;
	}
	if (compt > 0)
		return (1);
	return (0);
}

bool	check_line(char *line, int *i, int flag)
{
	if (flag == 1)
	{
		return (line[*i] != '\0' && line[*i] == '$'
			&& line[*i + 1] != '\0' && line[*i + 1] == '$');
	}
	else
	{
		return (line[*i] != '\0' && line[*i] == '$'
			&& line[*i + 1] != '\0' && line[*i + 1] == '?');
	}
}

int	prev_valo(char *lect)
{
	if (lect != NULL && ft_strlen(lect) >= 1
		&& lect[ft_strlen(lect) - 1] == '\n')
		return (1);
	return (0);
}

int	count_words(char const *str, char c)
{
	int	i;
	int	compteur;

	i = 0;
	compteur = 0;
	while (str[i] != '\0')
	{
		while (str[i] == c && str[i] != '\0')
			i++;
		if (str[i] != c && str[i] != '\0')
			compteur++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (compteur);
}

int	incrementspe(int i, char *line)
{
	if ((i + 1 < ft_strlen(line) && line[i] == '>' && line[i + 1] != '>')
		|| (i + 1 < ft_strlen(line) && line[i] == '<' && line[i + 1] != '<'))
		return (++i);
	else if (i + 2 < ft_strlen(line) && line[i] == '>' && line[i + 1] == '>'
		&& line[i + 2] != '>')
		return (i + 2);
	return (i);
}
