/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:52:26 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/22 18:48:44 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_destroy(t_env *env, char *unset)
{
	t_env			*tmp;

	while (env && env->next)
	{
		if (ft_strcmp(env->next->key, "PWD") == 0 && ft_strcmp(env->next->key, unset) == 0)
		{
			env->next->code = 4;
			return ;
		}
		if (!ft_strncmp(env->next->key, unset, ft_strlen(unset)))
		{
			tmp = env->next;
			env->next = tmp->next;
			if (tmp->key && tmp->key != NULL)
				free(tmp->key);
			if (tmp->value && tmp->value != NULL)
				free(tmp->value);
			free(tmp);
		}
		env = env->next;
	}
}

int	ft_unset(t_env *env, char **unset)
{
	unsigned int	i;

	i = 0;
	while (unset[++i])
	{
		if (ft_is_in_env(env, unset[i]))
			ft_destroy(env, unset[i]);
	}
	return (EXIT_SUCCESS);
}
