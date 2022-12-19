/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/19 13:27:58 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**path_generator(int path, char **env)
{
	if (path == -1)
		return (NULL);
	return (split_path(env[find_path(env)], ':'));
}

int	find_path(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (-1);
	while (env[i] != NULL)
	{
		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
			&& env[i][3] == 'H')
			return (i);
		i++;
	}
	return (-1);
}

char	**split_path(char *s, char c)
{
	char	**tab;
	int		a;
	int		i;
	int		d;

	i = 0;
	a = 0;
	tab = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!(tab))
		return (0);
	while (a < count_words(s, c) && s[i] != '\0')
	{
		tab[a] = malloc((words_length(s, i, c) + 2) * sizeof(char));
		if (!(tab[a]))
			return (free_tab(tab), (char **)0);
		d = 0;
		while (s[i] == c && s[i] != '\0')
			i++;
		while (s[i] != c && s[i] != '\0')
			tab[a][d++] = s[i++];
		tab[a][d++] = '/';
		tab[a][d] = '\0';
		a++;
	}
	return (tab[a] = 0, tab);
}