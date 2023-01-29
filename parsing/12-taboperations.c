/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12-taboperations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 11:52:25 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/29 23:16:51 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*tab_to_str(char **tabl, int opt)
{
	char	*str;
	int		i;

	i = 0;
	if (!tabl || tabl == NULL)
		return (NULL);
	str = ft_strdup(tabl[0]);
	if (str == NULL && tabl[0] != NULL)
		return (free_tab(tabl), NULL);
	if (tabl[0] == NULL && str == NULL)
		return (free_tab(tabl), str);
	while (tabl[++i] != NULL)
	{
		if (opt == 1)
			str = ft_strjoin_spe(str, " ");
		str = ft_strjoin_spe(str, tabl[i]);
		if (str == NULL)
			return (free_tab(tabl), NULL);
	}
	return (free_tab(tabl), str);
}

char	*ft_trim(char *s1)
{
	int		i;
	// char	*s1_bis;
	// char	**tabinter;

	i = -1;
	if (!s1 || s1 == NULL)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"')
			|| (s1[0] == '\'' && s1[1] == '\'')))
		return (s1);
	while (s1[++i] != '\0')
	{
		if (s1[i] == '\"')
			s1 = treat_str(s1, &i, '\"');
		else if (s1[i] == '\'')
			s1 = treat_str(s1, &i, '\'');
	}
	// tabinter = ft_split_spe(s1, '\v');
	// if (tabinter == NULL)
	// 	return (free(s1), NULL);
	// s1_bis = tab_to_str(tabinter, 0);
	return (trimator(s1));
}

char	*ft_trimhard(char *s1)
{
	int		i;
	//char	*s1_bis;
	//char	**tabinter;

	i = -1;
	if (!s1 || s1 == NULL)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"')
			|| (s1[0] == '\'' && s1[1] == '\'')))
		return (free(s1), NULL);
	while (s1[++i] != '\0')
	{
		if (s1[i] == '\"')
			s1 = treat_str(s1, &i, '\"');
		else if (s1[i] == '\'')
			s1 = treat_str(s1, &i, '\'');
	}
	// tabinter = ft_split_spe(s1, '\v');
	// if (tabinter == NULL)
	// 	return (free(s1), NULL);
	// s1_bis = tab_to_str(tabinter, 0);
	return (trimator(s1));
}

char	**trim_tab(char **tabl)
{
	int	i;

	i = -1;
	if (!tabl || tabl == NULL)
		return (NULL);
	while (tabl[++i] != NULL)
		tabl[i] = ft_trim(tabl[i]);
	return (tabl);
}

int	length_tab(char **tabl)
{
	int	i;

	i = 0;
	while (tabl[i] != NULL)
		i++;
	return (i);
}
