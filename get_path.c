/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/20 15:49:35 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*get_env_var(t_env *envp, char	*var)
{
	t_env *temp;

	if (!envp || envp == NULL)
		return (NULL);
	temp = envp;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->key, var, ft_strlen(var)) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

int is_whitespace(char c)
{
	if (c == 32 || (c>= 10 && c<=14))
		return (1);
	return (0);
}

int	ft_isprint(int c)
{
	if (c > 32 && c <= 126) //j'exclue le espace, pt a revoir dans les cas des quotes
		return (1);
	return (0);
}
