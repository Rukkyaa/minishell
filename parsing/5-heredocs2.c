/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-heredocs2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 21:56:54 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/29 23:30:23 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc_count(char *line, int index)
{
	int	i;
	int	compt;

	i = index;
	compt = 0;
	while (line[i] != '\0' && line[i + 1] != '\0' && line[i + 2] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		if (i == ft_strlen(line))
			break ;
		if (line[i] != '\0' && line[i] == '<' && line[i + 1] != '\0'
			&& line[i + 1] == '<' && line[i + 2] != '\0'
			&& line[i + 2] != '<')
			compt++;
		i++;
	}
	return (compt);
}

char	*clone(char *limiter)
{
	int	count;
	int	i;
	char *new_str;

	count = 0;
	i = 0;
	while (count < ft_strlen(limiter) && is_whitespace(limiter[i++]) == 0)
		count++;
	new_str = malloc((count + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = -1;
	while (++i < count)
		new_str[i] = limiter[i];
	return (new_str[i] = '\0', new_str);
}

char	*generate_name(char *limiter)
{
	char	*title;

	if (ft_strlen(limiter) == 0)
	{
		title = malloc(2);
		if (!title)
			return (free(limiter), NULL);
		title[0] = 'a';
		title[1] = '\0';
	}
	else
		title = clone(limiter);
	if (title == NULL)
		return (free(limiter), NULL);
	while (access(title, F_OK) == 0)
	{
		title = ft_strjoin_spe(title, "p");
		if (title == NULL)
			return (free(limiter), NULL);
	}
	return (free(limiter), title);
}

char	*find_lim2(char *line, int i, int compt, int d)
{
	char	*limiter;

	limiter = malloc((compt + 1) * sizeof(char));
	if (!limiter)
		return (NULL);
	compt = 0;
	while (line[i] != '\0' && i < d)
		limiter[compt++] = line[i++];
	return (limiter[compt] = '\0', ft_trimhard(limiter));
}

int	increment_i(char *line, int i)
{
	if (line[i] == '\"' || line[i] == '\'')
		return (avoid_quotes(line, i));
	return (++i);
}

char	*find_lim(char *line, int *alert)
{
	int		i;
	int		compt;
	int		d;

	i = 0;
	compt = 0;
	while (line[i] != '\0' && line[i + 1] != '\0'
		&& !(line[i] == '<' && line[i + 1] == '<'))
		i = increment_i(line, i);
	i += 2;
	while (line[i] != '\0' && is_whitespace(line[i]) == 1)
		i++;
	d = i;
	*alert = i;
	while (line[d] != '\0' && potential_name(line[i]) == 1)
	{
		if (line[d] == '\"' || line[d] == '\'')
		{
			compt += avoid_quotes(line, d) - 1 - d;
			d = avoid_quotes(line, d) - 1;
		}
		compt++;
		d++;
	}
	return (find_lim2(line, i, compt, d));
}
