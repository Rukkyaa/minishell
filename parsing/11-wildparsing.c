/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11-wildparsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 11:45:19 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/26 11:46:22 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	w_found(char *str)
{
	int	i;

	i = 0;
	if ((str[0] == '\'' || str[ft_strlen(str) - 1] == '\'')
		|| (str[0] == '\"' || str[ft_strlen(str) - 1] == '\"'))
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = avoid_quotes(str, i);
		if (str[i++] == '*')
			return (1);
	}
	return (0);
}

static char	*concat(char *s1)
{
	int	i;
	int	s;

	if (!s1)
		return (NULL);
	s = 0;
	while (s1[s] != '\0' && s1[s] == '*')
		s++;
	if (s > 0)
		s--;
	i = (int)(ft_strlen(s1) - 1);
	while (i > 0 && s1[i] == '*')
		i--;
	if (i != (int)(ft_strlen(s1) - 1))
		i++;
	return (ft_substr(s1, s, i + 1));
}

static char	**end_wild(char **tabfinal, char **tabl)
{
	char	*str;

	str = tab_to_str(tabfinal, 1);
	if (str == NULL)
		return (free_tab(tabfinal), free_tab(tabl), NULL);
	tabfinal = ft_split_spe(str, ' ');
	return (free(str), free_tab(tabl), tabfinal);
}

static char	*w_treat(char *tabli)
{
	char	*str;

	if (w_found(tabli) == 1)
	{
		str = wildcard(concat(tabli));
		if (str == NULL)
			return (ft_strdup(tabli));
		else
			return (str);
	}
	return (ft_strdup(tabli));
}

char	**w_finder(char **tabl)
{
	int		i;
	char	**tabfinal;

	i = 0;
	if (!tabl)
		return (NULL);
	tabfinal = malloc((length_tab(tabl) + 1) * sizeof(char *));
	if (!tabfinal)
		return (free_tab(tabl), NULL);
	while (tabl[i] != NULL)
	{
		tabfinal[i] = w_treat(tabl[i]);
		if (tabfinal[i++] == NULL)
			return (free_tab(tabfinal), free_tab(tabl), NULL);
	}
	return (tabfinal[i] = NULL, end_wild(tabfinal, tabl));
}
