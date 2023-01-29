/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-initbis2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 15:38:13 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/29 23:47:42 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*treat_str(char *s1, int *i, char c)
{
	s1[*i] = '\v';
	while (s1[*i] != '\0' && s1[*i] != c)
		(*i)++;
	if (s1[*i] == c)
		s1[*i] = '\v';
	return (s1);
}

char	**tabone(char *s)
{
	char	**tabl;

	tabl = malloc(2 * sizeof(char *));
	if (!tabl)
	{
		if (s != NULL)
			free(s);
		return (NULL);
	}
	if (s == NULL)
		tabl[0] = NULL;
	else
		tabl[0] = ft_strdup(s);
	if (s != NULL && tabl[0] == NULL)
		return (free(tabl), free(s), NULL);
	tabl[1] = NULL;
	return (free(s), tabl);
}

int	que_space(char *str)
{
	int	i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}
