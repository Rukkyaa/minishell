/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11-wildparsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 11:45:19 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/24 13:19:03 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	w_found(char *str)
{
	int i;

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

char **w_finder(char **tabl)
{
	int i;
	char **tabfinal;
	char *str;

	i = 0;
	tabfinal = malloc((length_tab(tabl) + 1) * sizeof(char *));
	if (!tabfinal)
		return (free_tab(tabl), NULL);
	while (tabl[i] != NULL)
	{
		if (w_found(tabl[i]) == 1)
		{
			str = wildcard(concat(tabl[i]));
			if (str == NULL)
				tabfinal[i] = ft_strdup(tabl[i]);
			else
			{
				tabfinal[i] = ft_strdup(str);
				free(str);
			}
			if (tabfinal[i] == NULL)
				return (free_tab(tabfinal), free_tab(tabl), NULL);
		}
		else
			tabfinal[i] = ft_strdup(tabl[i]);
		if (tabfinal[i++] == NULL)
			return (free_tab(tabfinal), free_tab(tabl), NULL);
	}
	tabfinal[i] = NULL;
	str = tab_to_str(tabfinal, 1);
	tabfinal = ft_split_spe(str, ' ');
	return (free(str), free_tab(tabl), tabfinal);
}
