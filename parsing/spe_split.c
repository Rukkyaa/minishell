/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spe_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:28:52 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/26 17:41:11 by axlamber         ###   ########.fr       */
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

// static char	**free_tab2(char **tabl, int a)
// {
// 	while (a >= 0)
// 	{
// 		if (tabl[a] != NULL)
// 			free(tabl[a]);
// 		a--;
// 	}
// 	if (tabl != NULL)
// 		free(tabl);
// 	return (0);
// }

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

int	check_spaces_spe(char *str, char c)
{
	int		i;
	char	*s;
	int		compt;

	i = 0;
	compt = 0;
	s = erase_redir(ft_strdup(str));
	while (s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '\"')
			compt++;
		if (s[i] != '\'' && s[i] != '\"' && s[i] != c)
			return (free(s), 0);
		i++;
	}
	if (compt > 0)
		return (free(s), 1);
	return (free(s), 0);
}

char	*gros_tas(char *s, int *i, char c)
{
	int		end;
	int		d;
	char	*str;

	str = malloc((words_length2(s, *i, c) + 1) * sizeof(char));
	if (!(str))
		return (NULL);
	d = 0;
	while (*i < ft_strlen(s) && s[*i] == c && s[*i] != '\0')
		(*i)++;
	while (*i < ft_strlen(s) && s[*i] != c && s[*i] != '\0')
	{
		if (s[*i] == '\"' || s[*i] == '\'')
		{
			end = avoid_quotes(s, *i);
			while (*i < ft_strlen(s) && s[*i] != '\0' && *i < end)
				str[d++] = s[(*i)++];
		}
		else
			str[d++] = s[(*i)++];
	}
	return (str[d] = '\0', str);
}

char	**ft_split_spe(char *s, char c)
{
	char	**tabl;
	int		a;
	int		i;
	int		end;

	i = 0;
	a = 0;
	end = 0;
	if (!s)
		return (NULL);
	tabl = malloc((count_words2(s, c) + 1) * sizeof(char *));
	if (!(tabl))
		return (NULL);
	while (a < count_words2(s, c) && i < ft_strlen(s) && s[i] != '\0')
	{
		tabl[a] = gros_tas(s, &i, c);
		if (tabl[a] == NULL)
			return (free_tab(tabl), NULL);
		a ++;
	}
	return (tabl[a] = 0, tabl);
}
