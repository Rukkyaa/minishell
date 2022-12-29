/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_epur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 19:11:54 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/29 10:39:11 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*ft_epur(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	char	*tmp;

	i = 0;
	j = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == ' ' && str[i + 1] == ' ')
			i++;
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	tmp = ft_strtrim(new_str, " ");
	free(new_str);
	free(str);
	return (tmp);
}

void	remove_sub(char **str, int start, int end)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (!new_str)
		return ;
	while ((*str)[i] != '\0')
	{
		if (i >= start && i <= end)
			i++;
		else
		{
			new_str[j] = (*str)[i];
			i++;
			j++;
		}
	}
	new_str[j] = '\0';
	free(*str);
	*str = ft_epur(new_str);
}