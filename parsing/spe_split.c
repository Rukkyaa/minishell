/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spe_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:28:52 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/10 19:33:43 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_words2(char *str, char c)
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
		{
			if (str[i] == '\"' || str[i] == '\'')
				i = avoid_quotes(str, i);
			else
				i++;
		}
	}
	return (compteur);
}

static int	words_length2(char *str, int i, char c)
{
	int	compteur;

	compteur = 0;
	while (str[i] == c && str[i] != '\0')
		i++;
	while (str[i] != c && str[i] != '\0')
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			compteur += avoid_quotes(str, i);
			i = avoid_quotes(str, i);
		}
		else
		{
			i++;
			compteur++;
		}
	}
	return (compteur);
}

static char	**free_tab2(char **tab, int a)
{
	while (a >= 0)
	{
		if (tab[a] != NULL)
			free(tab[a]);
		a--;
	}
	if (tab != NULL)
		free(tab);
	return (0);
}

char	**ft_split_spe(char *s, char c)
{
	char	**tab;
	int		a;
	int		i;
	int		d;
	int end;

	i = 0;
	a = 0;
	tab = malloc((count_words2(s, c) + 1) * sizeof(char *));
	if (!(tab))
		return (NULL);
	while (a < count_words2(s, c) && s[i] != '\0')
	{
		tab[a] = malloc((words_length2(s, i, c) + 1) * sizeof(char));
		if (!(tab[a]))
			return (free_tab2(tab, a));
		d = 0;
		while (s[i] == c && s[i] != '\0')
			i++;
		while (s[i] != c && s[i] != '\0')
		{
			if (s[i] == '\"' || s[i] == '\'')
			{
				end = avoid_quotes(s, i);
				while (s[i] != '\0' && i <= end)
					tab[a][d++] = s[i++];
			}
			else
				tab[a][d++] = s[i++];
		}
		tab[a][d] = '\0';
		a++;
	}
	tab[a] = 0;
	return (tab);
}