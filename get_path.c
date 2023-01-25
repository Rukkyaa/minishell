/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/24 21:35:32 by gatsby           ###   ########.fr       */
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
	if (c > 32 && c <= 126)
		return (1);
	return (0);
}
