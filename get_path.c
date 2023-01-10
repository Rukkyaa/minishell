/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/10 16:38:29 by gduhau           ###   ########.fr       */
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
char	*get_env_var(char	**envp, char	*var)
{
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
			return (envp[i] + ft_strlen(var) + 1);
	}
	return (NULL);
}
