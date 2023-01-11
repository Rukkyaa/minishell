/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/11 14:05:17 by gduhau           ###   ########.fr       */
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
	int		i;
	t_env *temp;

	i = -1;
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
