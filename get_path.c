/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/13 22:55:36 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
** Get the path from the environment variable
** and return it as a 2D array
** 
** @param   envp    The environment variable
**
** @return  The path as a 2D array
*/
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
