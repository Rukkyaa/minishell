/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:52:26 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/10 00:37:45 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_unset(t_env *env, char *to_unset)
{
	t_env	*tmp;

	while (env && env->next)
	{
		if (!ft_strncmp(env->next->key, to_unset, ft_strlen(to_unset)))
		{
			tmp = env->next;
			env->next = tmp->next;
			free(tmp);
			return (EXIT_SUCCESS);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
