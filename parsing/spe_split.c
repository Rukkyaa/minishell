/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spe_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:28:52 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/25 18:49:02 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_words2(char *str, char c)
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

static char	**free_tab2(char **tabl, int a)
{
	while (a >= 0)
	{
		if (tabl[a] != NULL)
			free(tabl[a]);
		a--;
	}
	if (tabl != NULL)
		free(tabl);
	return (0);
}

// char **tabone(char *s)
// {
// 	char **tabl;
	
// 	tabl = malloc(2 * sizeof(char *));
// 	if (!tabl)
// 		return (NULL);
// 	tabl[0] = ft_strdup(s);
// 	if (tabl[0] == NULL)
// 		return (NULL);
// 	tabl[1] = NULL;
// 	return (tabl);
// }

char	**ft_split_spe(char *s, char c)
{
	char	**tabl;
	int		a;
	int		i;
	int		d;
	int		end;

	i = 0;
	a = 0;
	if (!s)
		return (NULL);
	tabl = malloc((count_words2(s, c) + 1) * sizeof(char *));
	if (!(tabl))
		return (NULL);
	while (a < count_words2(s, c) && i < ft_strlen(s) && s[i] != '\0')
	{
		tabl[a] = malloc((words_length2(s, i, c) + 1) * sizeof(char));
		if (!(tabl[a]))
			return (free_tab2(tabl, a));
		d = 0;
		while (i < ft_strlen(s) && s[i] == c && s[i] != '\0')
			i++;
		while (i < ft_strlen(s) && s[i] != c && s[i] != '\0')
		{
			if (s[i] == '\"' || s[i] == '\'')
			{
				end = avoid_quotes(s, i);
				while (i < ft_strlen(s) && s[i] != '\0' && i <= end)
					tabl[a][d++] = s[i++];
			}
			else 
				tabl[a][d++] = s[i++];
		}
		tabl[a][d] = '\0';
		a++;
	}
	tabl[a] = 0;
	return (tabl);
}
