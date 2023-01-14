/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 12:13:55 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/13 23:04:25 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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

int	words_length(char const *str, int i, char c)
{
	int	compteur;

	compteur = 0;
	while (str[i] == c && str[i] != '\0')
		i++;
	while (str[i] != c && str[i++] != '\0')
		compteur++;
	return (compteur);
}

void	free_tab(char **tabl)
{
	int	i;

	i = 0;
	if (!tabl || tabl == NULL)
		return ;
	while (tabl[i])
		free(tabl[i++]);
	if (tabl != (char **)0)
		free(tabl);
}

/*
** Split function modified to add a '/' at the end of each string
**
** @param   s       The string to split
** @param   c       The character to split on
**
** @return  The string split as a 2D array with a '/' at the end of each string
*/
char	**split_path(char *s, char c)
{
	char	**tabl;
	int		a;
	int		i;
	int		d;

	i = 0;
	a = 0;
	if (!s || s == NULL)
		return (NULL);
	tabl = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!(tabl))
		return (0);
	while (a < count_words(s, c) && s[i] != '\0')
	{
		tabl[a] = malloc((words_length(s, i, c) + 2) * sizeof(char));
		if (!(tabl[a]))
			return (free_tab(tabl), (char **)0);
		d = 0;
		while (s[i] == c && s[i] != '\0')
			i++;
		while (s[i] != c && s[i] != '\0')
			tabl[a][d++] = s[i++];
		tabl[a][d++] = '/';
		tabl[a][d] = '\0';
		a++;
	}
	return (tabl[a] = 0, tabl);
}
