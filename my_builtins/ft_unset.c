/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 02:05:44 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/29 10:38:22 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_unset(t_env *env, char *to_unset)
{
	t_env	*tmp;

	while (env && env->next)
	{
		if (!strncmp(env->next->key, to_unset, ft_strlen(to_unset)))
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
